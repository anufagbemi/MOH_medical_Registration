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

#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "patient.h"
#include "patientHistory.h"
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <list>
#include <numeric>
#include <random>
#include "database.h"

//*************************************************************************************//
// Class for Setting up an appointment with a doctor. Program generates random slotss for meeting.
// This version of the software can only obtain the information from patient using command line
// instead of a GUI. The basic steps for setting up an appointment include.
//1. Loading day's appointment from database
//2. Input patient information
//3. Patient history Q&A
//4. Booking an appointment
// a. Doctor's availiability (e.g. random{this version} or calender)
// b. Patient chooses appointment from doctor's availiability
//5. Save patient info in database
//*************************************************************************************//


#define MINIMUM_TIME_SLOTS 4
#define APP_CALENDAR_SUCCESS 1
#define APP_CALENDAR_FAILURE 0
#define HOUR_TO_MINUTES 60
#define LUNCH_A_A -2
#define LUNCH_B_B -3
#define LUNCH_C_C -4
#define LUNCH_D_D -5
#define LUNCH_A 270
#define LUNCH_B 240

class Patient;

class Appointment
{
public:
    Appointment();
    Appointment                     (const char *dataBaseFile, Patient patientBio);
    virtual                         ~Appointment();

    /*------------------------------ Public member functions--------------------------------------------- */
    //                              Patient input. Order of entries is as follows: First Name, Last name, //
    //                              gender, email, phone number, occupation, date of birth, address1,     //
    //                              address2, City, State, Country, zip-code                              //
     /*-------------------------------------------------------------------------------------------------- */
    void                            patientInputDirect(Patient*  patientBio, const char* fN, const char* lN, const char* gd, const char* em, const char* ph,const char* occ,
                                    const char* dob, const char* r1, const char* r2, const char* city, const char* st, const char* cy, const char* zp);
    void                            initializeAppointment(const char*, DataBase *);
    void                            runPatient(Patient *patientBio, const char* dbFile, DataBase *patientDatabase);
    void                            patientInputCmd(Patient*  patientBio);
    void                            generateTimeSlots();
    int                             getDoctorsAppointmentFromExternalApp();

private:
    uint                            _meetingId=0;
    uint                            _meetingDuration=30;
    std::string                     _workStarts="0830";
    std::string                     _patientInputTimeS;
    uint                            _workStartHour;
    uint                            _workStartMinutes;
    uint                            _meetingIdB4LunchDel;
    uint                            _orgTotalSlotForDay;
    PatientHistory*                 _patientHist;
    double                          _workHours=9;
    double                          _maxWorkMinutes;
    int                             _lunchID;
    TimeMap                         _patientAvailability;
    TimeMap                         _doctorAvailabilities;
    TimeMap                         _dailyMeetingSlots;
    StringList                      _startHVec, _startMVec, _endHVec, _endMVec;
    bool                            _weekend;
    bool                            _break12to1;
    bool                            _break1230to130;
    bool                            _break12to1230;
    bool                            _noLunchbreak;
    bool                            _appointmentTimesConverted;

    // Member functions
    void reGenerateTimeSlots        (const char*, DataBase);
    bool appointmentFormatIsRight   (std::string);
    void setDoctorsAvailability     (std::string, int*, int orgSlotTotal);
    std::string swapChar            (std::string a);
    void setPatientsAppointment();
    void displayAvailableAppointments();
    void updateDoctorsAvailabilityDB_Beginning();

    // SQLite functions and containers
    std::vector<uint>               _meetingIdsToday;
    StringList                      _priorPatientAppointmentsToday;
    StringList                      _convertedAppointmentList;
    StringList                      convertTimeSlots(TimeMap, bool&);
    void saveAppointments           (const char*, DataBase*, Patient*, PatientHistory*);
    void getTodaysAppointments      (const char*, DataBase*, StringList&, std::vector<uint>&);
    void lunchTimeCorrection        (TimeMap&, std::string, int*);
    bool setWorkBreak               (std::string, int*);
    void uploadDoctorsResults       (std::string);
    void cancelAppointment          (std::string);
    void setAppointmentParameters   (std::string, uint, double);
    void udateAppointmentTimes();
    void checkAndUpdateAge();
};

#endif // APPOINTMENT_H
