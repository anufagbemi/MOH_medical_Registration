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


#include "patientHistory.h"


PatientHistory::PatientHistory() :
    _allergy(NULL),
    _pregnant(NULL),
    _anemia(NULL),
    _arthritis(NULL),
    _asthma(NULL),
    _diabetes(NULL),
    _hBloodPressure(NULL),
    _hCholesterol(NULL),
    _consumeAlcohol(NULL),
    _drinksFrequently(false),
    _smoke(NULL),
    _smokesFrequently(false),
    _firstTimeVisit(NULL)
{
}

void PatientHistory::initializeHistoryQA()
{

    std::cout << std::endl << std::endl;
    std::cout << "PATIENT MEDICAL HISTORY" << std::endl;
    printf("%s", "==========================================================\n");

    if (_firstTimeVisit==NULL)
    {
        _firstTimeVisit = new bool(false);
    }

    if (_allergy==NULL)
    {
        _allergy = new bool(false);
    }

    if (_pregnant==NULL)
    {
        _pregnant = new bool(false);
    }

    if (_anemia==NULL)
    {
        _anemia = new bool(false);
    }

    if (_arthritis==NULL)
    {
        _arthritis = new bool(false);
    }

    if (_asthma==NULL)
    {
        _asthma = new bool(false);
    }

    if (_diabetes==NULL)
    {
        _diabetes = new bool(false);
    }

    if (_hBloodPressure==NULL)
    {
        _hBloodPressure = new bool(false);
    }

    if (_hCholesterol==NULL)
    {
        _hCholesterol = new bool(false);
    }

    if (_consumeAlcohol==NULL)
    {
        _consumeAlcohol = new bool(false);
    }

    if (_smoke==NULL)
    {
        _smoke = new bool(false);
    }

    patHistData.firstTimeVisitQA = "First time visit to this clinic? (Y/N)";
    patHistData.alergyQA         = "Are you allergic to anything? (Y/N)";
    patHistData.pregnancyQA      = "Are you pregnant? (Y/N)" ;
    patHistData.anemiaQA         = "Any prior diagnosis of anemia? (Y/N)";
    patHistData.arthritisQA      = "Any prior diagnosis of arthritis? (Y/N)";
    patHistData.asthmaQA         = "Any prior diagnosis of asthma? (Y/N)" ;
    patHistData.diabetesQA       = "Any prior diagnosis of diabetes? (Y/N)";
    patHistData.bloodPressureQA  = "Any family history of high blood pressure? (Y/N)";
    patHistData.colestorolQA     = "Any history of high colestorol? (Y/N)";
    patHistData.alcoholQA        = "Do you consume alcohol? (Y/N)";
    patHistData.smokingQA        = "Do you smoke? (Y/N)";


    // store the questions into container
    _qAConditions[patHistData.firstTimeVisitQA ] = _firstTimeVisit;
    _qAConditions[patHistData.alergyQA         ] = _allergy;
    _qAConditions[patHistData.pregnancyQA      ] = _pregnant;
    _qAConditions[patHistData.anemiaQA         ] = _anemia;
    _qAConditions[patHistData.arthritisQA      ] = _arthritis;
    _qAConditions[patHistData.asthmaQA         ] = _asthma;
    _qAConditions[patHistData.diabetesQA       ] = _diabetes;
    _qAConditions[patHistData.bloodPressureQA  ] = _hBloodPressure;
    _qAConditions[patHistData.colestorolQA     ] = _hCholesterol;
    _qAConditions[patHistData.alcoholQA        ] = _consumeAlcohol;
    _qAConditions[patHistData.smokingQA        ] = _smoke;
}


void PatientHistory::checkBoolInput(char* cValue, std::pair<std::string, bool*> pValue)
{
    int i=0;
    while (i<I_MAX)
    {
        std::strcpy(cValue, "N");
        std::cout << pValue.first << ": " ;
        std::cin.getline(cValue, I_MAX);
        if((strcasecmp(cValue, "Y")==0) || (strcasecmp(cValue, "yes")==0))
        {
            *pValue.second = true;
            break;
        }
        if((strcasecmp(cValue, "N")==0) || (strcasecmp(cValue, "no")==0))
        {
            break;
        }
        if(((strcasecmp(cValue, "Y")!=0) && (strcasecmp(cValue, "yes")!=0)) &&
            ((strcasecmp(cValue, "N")!=0) && (strcasecmp(cValue, "no")!=0)))
        {
            std::cerr << "Please enter a correct value." << std::endl;
            i++;
        }
    }
}

void PatientHistory::patientHistoryQA()
{
    char dummyVar[MAX_BOOL_LEN];

    for (auto itr=_qAConditions.begin(); itr!=_qAConditions.end(); ++itr )
    {
        checkBoolInput(dummyVar, std::make_pair(itr->first, itr->second));

        if(itr->first==patHistData.firstTimeVisitQA)
        {
            std::cout << "Reason for visit: ";
            std::getline(std::cin, _reasonForVisit);

            if(!(*_firstTimeVisit))
            {
                uint i=0;
                while (i<I_MAX)
                {
                    std::cout << "Previous lab visit? (DD/MM/YYYY): " ;
                    std::getline(std::cin, _lastLabVisit);

                    if (DateFormatIsRight(_lastLabVisit))
                    {
                        resetId(i);
                        break;
                    }
                    else
                    {
                        i++;
                    }
                }

                std::cout << "Lab tests done. (separated by commas) (If no test, type none): ";
                std::getline(std::cin >> std::ws, _labTests);

                std::vector<std::string> tests = stringSplit(_labTests, ",");

                _labTests.clear(); ;

                for (uint i=0;i<tests.size();i++)
                {
                    for (uint j=0;j<tests[i].size();j++)
                    {
                        if(tests[i][j] != ' ')
                            _labTests += tests[i][j];
                    }
                    if (i!=tests.size()-1)
                        _labTests += ", ";
                }
                break;
            }
        }

        if(itr->first==patHistData.alergyQA)
        {
            if(*_allergy)
            {
                std::cout << "What types? (separated by commas): " ;
                std::getline(std::cin, _allergyTypes);

                std::vector<std::string> allergyTmp = stringSplit(_allergyTypes, ", ");

                _allergyTypes.clear();

                for (uint i=0;i<allergyTmp.size();i++)
                {
                    for (uint j=0;j<allergyTmp[i].size();j++)
                    {
                        if(allergyTmp[i][j] != ' ')
                            _allergyTypes += allergyTmp[i][j];
                    }
                    if (i!=allergyTmp.size()-1)
                        _allergyTypes += ", ";
                }
            }
            std::string gender = getGender();
            if(strcasecmp(gender.c_str(), "Female")!=0 && strcasecmp(gender.c_str(), "F"))
            {
                auto id = _qAConditions.find(patHistData.pregnancyQA);
                _qAConditions.erase(id);
            }
        }

        if(itr->first==patHistData.alcoholQA)
        {
            if(*_consumeAlcohol)
            {
                std::cout << "How often from a scale of 1 - 10 with 1 being a little, 10 being very frequently? ";
                std::getline(std::cin, _alcoholFrequency);

                if (stoi(_alcoholFrequency) > 5)
                    _drinksFrequently = true;
            }
        }

        if(itr->first==patHistData.smokingQA)
        {
            if(*_smoke)
            {
                std::cout << "How often from a scale of 1 - 10 with 1 being a little, 10 being very frequently? ";
                std::getline(std::cin, _smokingFrequency);

                if (stoi(_smokingFrequency) > 5)
                    _smokesFrequently = true;
            }
        }
    }

    this->updatePatientHistoryData();
}

void PatientHistory::updatePatientHistoryData()
{
    if(*_firstTimeVisit)
        patHistData.firstTimeVisit   = TRUE;
    if(*_anemia)
        patHistData.anemia           = TRUE;
    if(*_arthritis)
        patHistData.arthritis        = TRUE;
    if(*_asthma)
        patHistData.asthma           = TRUE;
    if(*_diabetes)
        patHistData.diabetes         = TRUE;
    if(_drinksFrequently)
        patHistData.drinksFrequently = TRUE;
    if(_smokesFrequently)
        patHistData.smokesFrequently = TRUE;
    if(*_firstTimeVisit)
        patHistData.firstTimeVisit   = TRUE;
    if(*_hBloodPressure)
        patHistData.hBloodPressure   = TRUE;
    if(*_hCholesterol)
        patHistData.hCholesterol     = TRUE;
    if(*_pregnant)
        patHistData.pregnant         = TRUE;
}

bool PatientHistory::firstTimeVisit() const
{
    return _firstTimeVisit;
}

std::string PatientHistory::previousLabAppointment() const
{
    return _lastLabVisit;
}

std::string PatientHistory::getPreviousLabTestsDone() const
{
    return _labTests;
}

std::string PatientHistory::getReasonForVisit() const
{
    return _reasonForVisit;
}

std::string PatientHistory::getAllergyList()
{
    return _allergyTypes;
}


PatientHistory::~PatientHistory()
{
    for (PatHistoryMap::iterator itr=_qAConditions.begin(); itr!=_qAConditions.end(); ++itr )
    {
        delete (itr->second);
    }
    _qAConditions.clear();
}
