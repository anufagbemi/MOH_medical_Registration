/*
The code is licensed under the MIT License <http://opensource.org/licenses/MIT>:

Copyright (c) 2023 Samuel Fagbemi.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "appointment.h"
#include "patient.h"
#include <numeric>

Appointment::~Appointment()
{ delete _patientHist;
}


Appointment::Appointment():
    _meetingDuration(30),
    _workStarts("0830"),
    _weekend(false),
    _break12to1(true),
    _break1230to130(false),
    _break12to1230(false),
    _noLunchbreak(false),
    _appointmentTimesConverted(false)
{
}


void Appointment::initializeAppointment(const char* dbFile, DataBase* patientDatabase)
{
    patientDatabase->createTable(dbFile);

    StringList date, dayStart, patientAppointments;
    std::vector<uint> totalSlots, meetingIncrement, lunchId;

    getTodaysAppointments(dbFile, patientDatabase,
                          patientAppointments, lunchId);
}

void Appointment::patientInputDirect(Patient*  patientBio, const char* fN, const char* lN, const char* gd, const char* em,
                                     const char* ph,const char* occ, const char* dob, const char* r1, const char* r2,
                                     const char* city, const char* st, const char* cy, const char* zp)  // Default
{

    patientBio->setPatientData(fN, lN, gd, em, ph, dob, occ);
    patientBio->setResidentialAddress(r1, r2, city, st, zp, cy, true);

    patientBio->createPatientInfo();
}

void Appointment::patientInputCmd(Patient *patientBio)
{
    // Step 2.
    if(!patientBio->getFirstName().empty()|| !patientBio->getLastName().empty() ||
        !patientBio->getGender().empty()) // can add others if desired
    {
        std::cerr << "Data already populated. Skipping... " << std::endl;
        return;
    }
    patientBio->populatePatientInfoFromCmdLine();
    patientBio->createPatientInfo();
}

void Appointment::runPatient(Patient*  patientBio, const char* dbFile, DataBase* patientDatabase)
{

    if(_meetingDuration>60)
    {
        std::cerr << "Please set meeting duration for 60 min or less." ;
        exit(EXIT_FAILURE);
    }

    _maxWorkMinutes = _workHours*HOUR_TO_MINUTES;

    // Patient medical history
    _patientHist = new PatientHistory();
    _patientHist->initializeHistoryQA();
    _patientHist->patientHistoryQA();

    // Setting up appointment
    std::string tmp;
    if(_workStarts[0]=='0')
    {
        tmp = _workStarts[1];
        _workStartHour= stoi(tmp); tmp.clear();
    }
    else
    {
        for (int i=0; i< mCOUNT-1; ++i)
        {
            tmp += _workStarts[i];
            _workStartHour= stoi(tmp); tmp.clear();
        }
    }

    bool found=0;
    auto loc=_workStarts.find(":");

    if (loc != std::string::npos)
    {
        found=true;
    }

    if(found)
    {
        _workStarts.erase(_workStarts.begin()+loc);
    }

    for (int i=mCOUNT-1; i< mCOUNT+1; ++i)
    {
        tmp += _workStarts[i];
        _workStartMinutes= stoi(tmp);
    }

    this->generateTimeSlots();

    this->setDoctorsAvailability("", &_lunchID, _orgTotalSlotForDay);

    this->displayAvailableAppointments();

    this->setPatientsAppointment();

    // Save data and appointment information
    saveAppointments(dbFile, patientDatabase, patientBio, _patientHist);
    delete patientBio;
    delete patientDatabase;
}

void Appointment::saveAppointments(const char* dbFile, DataBase* patientDatabase,
                                   Patient* patient, PatientHistory* patientHist)
{
    std::string workStarts1=_startHVec[0]+":"+_startMVec[0];
    uint totalAvailableSlots= _startHVec.size();

    patientDatabase->insertPatientInfo(
        dbFile,
        patient,
        patientHist,
        workStarts1,
        _meetingDuration,
        _meetingId,
        totalAvailableSlots,
        _patientInputTimeS
        );
}

void Appointment::getTodaysAppointments(const char* dbFile, DataBase* patientDatabase,
                                        StringList& patientAppointments, std::vector<uint>& lunchId)
{
    StringList date, dayStart;
    std::vector<uint> totalSlots, meetingIncrement;

    int firstAptIdToday;

    patientDatabase->getSQLData(
        dbFile,
        date,
        dayStart,
        totalSlots,
        meetingIncrement,
        lunchId,
        patientAppointments
        );

    std::string date_t = _patientHist->todaysDate();

    if(date.size()==0)
        return;

    int tmp=0;
    for (uint i=0; i<date.size(); i++)
    {
        if (date_t ==  date[i])
        {
            tmp++;
            _priorPatientAppointmentsToday.push_back(patientAppointments[i]);
            if(tmp==1)
                firstAptIdToday =i;  // first patient information for the current day.
        }
    }

    if(firstAptIdToday>=0)
    {
        _workStarts = dayStart[firstAptIdToday];
        _meetingDuration = meetingIncrement[firstAptIdToday];
        _orgTotalSlotForDay = totalSlots[firstAptIdToday];
        _lunchID = lunchId[firstAptIdToday];
    }
}

void Appointment::udateAppointmentTimes()
{

    _convertedAppointmentList = convertTimeSlots(_doctorAvailabilities, _appointmentTimesConverted);

    StringList tmpVec;

    std::vector<uint> delID;
    for(uint i=0; i<_priorPatientAppointmentsToday.size(); i++)
    {
        int foundId=0;
        for(uint j=0; j<_convertedAppointmentList.size(); j++)
        {
            if(_convertedAppointmentList[j] == _priorPatientAppointmentsToday[i])
            {
                foundId = j;
                break;
            }
        }
        _startHVec.erase(_startHVec.begin()+foundId);
        _startMVec.erase(_startMVec.begin()+foundId);
        _endHVec.erase(_endHVec.begin()+foundId);
        _endMVec.erase(_endMVec.begin()+foundId);
        _convertedAppointmentList.erase(_convertedAppointmentList.begin()+foundId);
    }

    if(_startHVec.size()==1)
    {
        std::cout << "Only one appointment left for the day." << std::endl << std::endl;
    }
    if(_startHVec.size()==0)
    {
        std::cout << "No appointments available again for today. Please try again tomorrow."
                  << std::endl << std::endl;
    }
}

void Appointment::setDoctorsAvailability(std::string breakTime, int* lunchID, int orgSlotTotal)
{

    if(_priorPatientAppointmentsToday.size()>0) // if there is DB info for that day
    {
        assert(!_dailyMeetingSlots.empty());
        _doctorAvailabilities = _dailyMeetingSlots;
        this->lunchTimeCorrection(_doctorAvailabilities, breakTime, lunchID);

        //Reduce the generated dictionary to original size at day's first appointment.
        //Cut from the bottom to endpoint
        uint SIZE=_doctorAvailabilities.size();
        uint delIDs = orgSlotTotal;

        std::vector<uint> startHrs;
        for (TimeMap::iterator id=_doctorAvailabilities.begin();
             id != _doctorAvailabilities.end(); ++id)
        {
            startHrs.push_back(id->first);
        }

        uint k=SIZE-1;
        for (uint j=0; j<SIZE; j++)
        {
            auto id = _doctorAvailabilities.find(startHrs[k]);
            _doctorAvailabilities.erase(id->first);
            k--;
            if(k==delIDs-1) break;
        }

        this->udateAppointmentTimes();
    }
    else
    {
        if (getDoctorsAppointmentFromExternalApp() == APP_CALENDAR_SUCCESS)
        {
            // not available in demo version
        }
        else
        {
            // Generate random doctor appointment times
            srand(time(0));

            int ub = _dailyMeetingSlots.size();

            uint randCount = (rand() % (ub - MINIMUM_TIME_SLOTS + 1)) + MINIMUM_TIME_SLOTS;

            std::vector<uint> indices(randCount);
            std::iota(indices.begin(), indices.end(), 0);
            std::random_shuffle(indices.begin(), indices.end());
            sort(indices.begin(), indices.end());

            uint k=0;
            for (TimeMap::iterator id=_dailyMeetingSlots.begin();
                id != _dailyMeetingSlots.end(); ++id)
            {
                if(k==indices[k])
                    _doctorAvailabilities.insert(std::make_pair(id->first, id->second));

                k++;
            }
            this->lunchTimeCorrection(_doctorAvailabilities, breakTime, lunchID);

            this->convertTimeSlots(_doctorAvailabilities, _appointmentTimesConverted);
        }
    }
}


void Appointment::lunchTimeCorrection(TimeMap& doctorAvailabilities, std::string breakTime, int* lunchID)
{

    uint lunchTimesArrSize=HOUR_TO_MINUTES/_meetingDuration;

    if(lunchTimesArrSize<2)
    {
        lunchTimesArrSize=mCOUNT-1;
    }

    if (_workStartMinutes > 30)
    {
        lunchTimesArrSize++;
    }

    int lunchCorrection= (_workStartHour-8)*HOUR_TO_MINUTES;
    lunchCorrection += _workStartMinutes;

    int tmpDuration=_meetingDuration;
    if (tmpDuration<1)
    {
        tmpDuration=1;
    }
    uint lunchStartTime;

    if(!this->setWorkBreak(breakTime, lunchID))
    {
        if (_patientHist->verbose)
        {
            std::cerr << "Lunch Time ID not set. Using default, 12-1PM." << std::endl;
        }
    }

    if (_break1230to130)
    {
        lunchStartTime=LUNCH_A;
        _lunchID = LUNCH_A_A;
    }
    if (_break12to1)
    {
        lunchStartTime=LUNCH_B;
        _lunchID = LUNCH_B_B;
    }

    if (_break12to1230)
    {
        lunchStartTime=LUNCH_B;
        lunchTimesArrSize/=2;
        _lunchID = LUNCH_C_C;
    }
    if (_noLunchbreak)
    {
        lunchTimesArrSize=0;
        _lunchID = LUNCH_D_D;
    }

    // correction for workday assuming it starts at 8 AM
    lunchStartTime -= lunchCorrection;

    std::vector<uint> lunchTimes(lunchTimesArrSize);

    for (uint i=0; i<lunchTimesArrSize; i++)
    {
        lunchTimes[i] = lunchStartTime+(i*tmpDuration);
    }

    uint priorSize = doctorAvailabilities.size();
    for (uint j=0; j<lunchTimesArrSize; j++)
    {
        auto id = doctorAvailabilities.find(lunchTimes[j]);
        doctorAvailabilities.erase(id->first);
    }
    if (lunchTimesArrSize !=0 && (priorSize == doctorAvailabilities.size()))
    {
        TimeMap docTemp = doctorAvailabilities;

        for (uint j=0; j<lunchTimesArrSize; j++)
        {
            uint lunchId = lunchTimes[j];

            for (TimeMap::iterator itr=docTemp.begin();
                 itr != docTemp.end(); ++itr)
            {
                auto id = doctorAvailabilities.find(itr->first);

                if((lunchId - id->first <= _meetingDuration))
                {
                    doctorAvailabilities.erase(id->first);
                }
            }
        }
    }
}

int Appointment::getDoctorsAppointmentFromExternalApp()
{
    return APP_CALENDAR_FAILURE;  // or APP_CALENDAR_SUCCESS
}

bool Appointment::appointmentFormatIsRight(std::string patientInputTimeS)
{

    std::string tmpInput;

    for (uint i =0; i< patientInputTimeS.size(); ++i)
    {
        if(patientInputTimeS[i] != ' ')
        {
            tmpInput.push_back(patientInputTimeS[i]);
        }
    }
    if (!tmpInput.empty())
    {
        patientInputTimeS=tmpInput;
    }

    if (patientInputTimeS.size() != 11)
    {
        std::cerr << "Please enter right availability. " << std::endl;
        return false;
    }

    // hyphen
    if(patientInputTimeS[yCOUNT-1] != '-')
    {
        std::cerr << "Please enter right availability." << std::endl;
        return false;
    }

    // positions of numbers
    for (uint i=0; i < patientInputTimeS.size(); i++)
    {
        if((i==0 && i<mCOUNT-1) || (i==mCOUNT && i<yCOUNT-1) || (i==yCOUNT && i<zCOUNT-1)
            || (i==zCOUNT && i<zCOUNT+2))
        {
            if(!_patientHist->isANumber(patientInputTimeS[i]))
            {
                std::cerr << "Please enter right availability." << std::endl;
                return false;
            }
        }

        // semi-colons
        if((i==mCOUNT-1) || (i==zCOUNT-1) )
        {
            if(patientInputTimeS[i] != ':')
            {
                std::cerr << "Please enter right availability." << std::endl;
                return false;
            }
        }
    }

    // Checking the values for availability
    std::string appointmentHStart, appointmentHEnd,
                appointmentMStart, appointmentMEnd;
    std::string tmp;

    for (int i=0; i< mCOUNT-1; ++i)
    {
        tmp += patientInputTimeS[i];
    }appointmentHStart= tmp; tmp.clear();

    for (int i=mCOUNT; i< yCOUNT-1; ++i)
    {
        tmp += patientInputTimeS[i];
    }appointmentMStart= tmp; tmp.clear();

    for (int i=yCOUNT; i< zCOUNT-1; ++i)
    {
        tmp += patientInputTimeS[i];
    }appointmentHEnd= tmp; tmp.clear();

    for (int i=zCOUNT; i< zCOUNT+2; ++i)
    {
        tmp += patientInputTimeS[i];
    } appointmentMEnd= tmp;

    // Check that time slot exists
    std::vector<std::string>::iterator iterHS, iterHE, iterMS, iterME;

    iterHS = find(_startHVec.begin(), _startHVec.end(), appointmentHStart);
    iterMS = find(_startMVec.begin(), _startMVec.end(), appointmentMStart);
    iterHE = find(_endHVec.begin(), _endHVec.end(), appointmentHEnd);
    iterME = find(_endMVec.begin(), _endMVec.end(), appointmentMEnd);

    if(iterHS == _startHVec.end() && iterHE == _endHVec.end() &&
       iterMS == _startMVec.end() && iterME == _endMVec.end() )
    {
        std::cerr << "Time slot is not available. Please choose another time!" << std::endl;
        return false;
    }

    // Second check that it matches exact time slot
    assert((_startHVec.size()==_startMVec.size()) && (_endHVec.size()==_endMVec.size()));

    for (uint i=0; i<_startHVec.size();i++)
    {
        if(appointmentHStart ==_startHVec[i] && appointmentMStart ==_startMVec[i])
        {
            _meetingId = i;
            break;
        }
    }

    if(appointmentHEnd != _endHVec[_meetingId] || appointmentMEnd !=_endMVec[_meetingId])
    {
        std::cerr << "Time slot is not available. Please choose another time!" << std::endl;
        return false;
    }
    return true;
}

void Appointment::setPatientsAppointment()
{

    uint i=0, imax=20;

    std::cout << "24-hr format e.g. 07:00-07:45 or 13:00-13:30" << std::endl;

    // Time must match available time
    while(i<imax)
    {
        std::getline(std::cin, _patientInputTimeS);

        if(appointmentFormatIsRight(_patientInputTimeS))
        {
            std::cout << std::endl;
            std::cout << "+++++++++++++++++++++++++++++++++" << std::endl;
            std::cout << "Appointment successfully created." << std::endl;
            std::cout << "+++++++++++++++++++++++++++++++++" << std::endl;
            break;
        }
        else
        {
            i++;
        }
    }
}

bool Appointment::setWorkBreak(std::string breakTime, int* lunchID)
{

    if (!breakTime.empty())
    {
        if (breakTime == "12:30-1:30PM" ) { _break1230to130 = true; }
        if (breakTime == "12:00-1:00PM" ) { _break12to1     = true; }
        if (breakTime == "12:00-12:30PM") { _break12to1230  = true; }
        if (breakTime == "None"         ) { _noLunchbreak   = true; }
        return true;
    }
    if (lunchID != NULL)
    {
        if(*lunchID == LUNCH_A_A)  { _break1230to130= true; }
        if(*lunchID == LUNCH_B_B)  { _break12to1    = true; }
        if(*lunchID == LUNCH_C_C)  { _break12to1230 = true; }
        if(*lunchID == LUNCH_D_D)  { _noLunchbreak  = true; }
        return true;
    }
    return false;
}

StringList Appointment::convertTimeSlots(TimeMap availabilities, bool& appointmentTimesConverted)
{
    StringList timeSlotsStr;
    if(!appointmentTimesConverted)
    {

        std::vector<uint> docAvailabiltyStart;
        std::vector<uint> docAvailabiltyEnd;

        for (TimeMap::iterator id=availabilities.begin();
             id != availabilities.end(); ++id)
        {
            docAvailabiltyStart.push_back(id->first);
            docAvailabiltyEnd.push_back(id->second);
        }

        for(uint i=0; i<docAvailabiltyStart.size(); i++)
        {
            uint startHour    = _workStartHour    + docAvailabiltyStart[i]/HOUR_TO_MINUTES;
            uint startMinutes = _workStartMinutes + docAvailabiltyStart[i] % HOUR_TO_MINUTES;

            uint endHour      = _workStartHour    + docAvailabiltyEnd[i]/HOUR_TO_MINUTES;
            uint endMinutes   = _workStartMinutes + docAvailabiltyEnd[i] % HOUR_TO_MINUTES;

            if(startMinutes >= HOUR_TO_MINUTES)
            {
                startMinutes -= HOUR_TO_MINUTES;
                startHour++;
            }

            if(endMinutes >= HOUR_TO_MINUTES)
            {
                endMinutes -= HOUR_TO_MINUTES;
                endHour++;
            }

            std::string startHourS=std::to_string(startHour), startMinutesS=std::to_string(startMinutes),
                endHourS=std::to_string(endHour), endMinutesS=std::to_string(endMinutes);

            if(startHourS.size()<2)
            {
                startHourS.append("0");
                startHourS = swapChar(startHourS);
            }
            if(startMinutesS.size()<2)
            {
                startMinutesS.append("0");
                startMinutesS = swapChar(startMinutesS);
            }
            if(endHourS.size()<2)
            {
                endHourS.append("0");
                endHourS = swapChar(endHourS);
            }
            if(endMinutesS.size()<2)
            {
                endMinutesS.append("0");
                endMinutesS = swapChar(endMinutesS);
            }
            std::string tmpF = startHourS+":"+startMinutesS+"-"+
                               endHourS  +":"+endMinutesS;

            timeSlotsStr.push_back(tmpF);

            _startHVec.push_back(startHourS);
            _startMVec.push_back(startMinutesS);
            _endHVec.push_back(endHourS);
            _endMVec.push_back(endMinutesS);
        }
        appointmentTimesConverted = true;
    }
    else
    {
        return {""};
    }
    return timeSlotsStr;
}

std::string Appointment::swapChar(std::string a)
{
    std::string finalSwap= {a[1], a[0]};

    return finalSwap;
}


void Appointment::displayAvailableAppointments()
{

    // Main process is for patient to choose time slots
    std::cout << std::endl << std::endl;
    std::cout << "APPOINTMENT TIME SELECTION" << std::endl;
    printf("%s", "==========================================================\n");

    std::cout<< "Available time slots: " << std::endl;

    assert(!_startHVec.empty()||!_startMVec.empty()||!_endHVec.empty()||!_endMVec.empty());
    for(uint i=0; i<_startHVec.size(); i++)
    {
        std::cout << _startHVec[i] << ":" << _startMVec[i] << "-"
                  << _endHVec[i]   << ":" << _endMVec[i] << std::endl;
    }

    std::cout<<std::endl << "Please choose from the above appointment times in ";
}

void Appointment::generateTimeSlots()
{

    uint maxDailyMeetingTimes;

    if(_noLunchbreak)
    {
        maxDailyMeetingTimes = double(_maxWorkMinutes/_meetingDuration);
    }
    if(_break12to1)
    {
        maxDailyMeetingTimes = double(_maxWorkMinutes/_meetingDuration)-1;
    }
    if(_break1230to130)
    {
        maxDailyMeetingTimes = double(_maxWorkMinutes/_meetingDuration)-1;
    }
    if(_break12to1230)
    {
        maxDailyMeetingTimes = double(_maxWorkMinutes/_meetingDuration);
    }

    for (uint i=1; i<maxDailyMeetingTimes+1; i++)
    {
        _dailyMeetingSlots.insert(std::make_pair(_meetingDuration*(i-1), _meetingDuration*i));
    }
}

void Appointment::setAppointmentParameters(std::string startTime, uint interval, double workHours)  // Daily only
{
    _meetingDuration = interval;
    _workStarts = startTime;
    _workHours = workHours;
}
