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

#ifndef PATIENTHISTORY_H
#define PATIENTHISTORY_H

#include "patient.h"
#include "fifo_map.hpp"

//*************************************************************************************//
// This class queries patient for medical history before setting an appointment
// to see a doctor.
// This version of the software is a demo that shows common questions asked before
// seeing a physician. It can only obtain the information from patient
// using command line instead of a GUI.
//
//*************************************************************************************//


#define MAX_BOOL_LEN 6


struct PatientHistoryData
{

    std::string firstTimeVisitQA;
    std::string alergyQA;
    std::string pregnancyQA;
    std::string anemiaQA;
    std::string arthritisQA;
    std::string asthmaQA;
    std::string diabetesQA;
    std::string bloodPressureQA;
    std::string colestorolQA;
    std::string alcoholQA;
    std::string smokingQA;
    std::string generalQA;

    char anemia           =FALSE;
    char arthritis        =FALSE;
    char allergies        =FALSE;
    char pregnant         =FALSE;
    char asthma           =FALSE;
    char diabetes         =FALSE;
    char hBloodPressure   =FALSE;
    char hCholesterol      =FALSE;
    char drinksFrequently =FALSE;
    char smokesFrequently =FALSE;
    char firstTimeVisit   =FALSE;
};

class PatientHistory : public Patient
{

public:
    PatientHistory();
    virtual ~PatientHistory();

    using               PatHistoryMap = nlohmann::fifo_map<std::string, bool*>;
    bool                firstTimeVisit() const;
    PatientHistoryData  patHistData;
    std::string         getPreviousLabTestsDone() const;
    std::string         previousLabAppointment() const;
    std::string         getReasonForVisit() const;
    std::string         getAllergyList();
    void                patientHistoryQA();
    void                initializeHistoryQA();

private:

    std::string         _allergyTypes;
    std::string         _alcoholFrequency;
    std::string         _smokingFrequency;

    //prior disease check list
    bool*               _allergy;
    bool*               _pregnant;
    bool*               _anemia;
    bool*               _arthritis;
    bool*               _asthma;
    bool*               _diabetes;
    bool*               _hBloodPressure;
    bool*               _hCholesterol;
    bool*               _consumeAlcohol;
    bool                _drinksFrequently;
    bool*               _smoke;
    bool                _smokesFrequently;
    bool*               _firstTimeVisit;
    std::string         _lastLabVisit;
    std::string         _labTests;
    std::string         _reasonForVisit;
    PatHistoryMap       _qAConditions;
    void                updatePatientHistoryData();
    void                checkBoolInput(char*, std::pair<std::string, bool *>);

};

#endif // PATIENTHISTORY_H
