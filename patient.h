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

#ifndef PATIENT_H
#define PATIENT_H

#include <functional>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <map>
#include <regex>


#include <functional>

//*************************************************************************************//
// This class obtains patient's data before setting an appointment to see a doctor.
// This version of the software can only obtain the information from patient using command line
// or by setting using functions directly within the code instead of a GUI.
//
// Some of the information collected are basic information such as
// 1.Full Name, 2.date or birth, 3.address, 4.email, 5. phone number, etc.
//*************************************************************************************//


typedef unsigned int uint;
typedef std::map<uint, uint> TimeMap;
typedef std::vector<std::string> StringList;
typedef std::vector<std::vector<std::string>> StringListList;

#define I_MAX  20
#define TRUE   'Y'
#define FALSE   'N'
#define WCHAR_SIZE 21
#define ALPHA_SIZE 26
#define NUM_SIZE 10
#define mCOUNT 3
#define yCOUNT 6
#define zCOUNT 9
#define YEAR_REF 1900
#define MAX_NAME_LEN 60
#define MAX_ZIP_LEN 30
#define MAX_ADDRESS_LEN 120
#define MAX_GENDER_LEN 10
#define MAX_DOB_LEN 11
#define MAX_PHONE_LEN 100
#define DATE_ERROR    (std::cerr << "Please enter a valid date. " << std::endl)
#define INPUT_ERROR   (std::cerr << "Please enter a valid option. " << std::endl)
#define DOB_ERROR     (std::cerr << "Please enter a valid date of birth option. " << std::endl)
#define EMAIL_ERROR   (std::cerr << "Please enter a valid email. " << std::endl)
#define GEND_ERROR    (std::cerr << "Please enter a valid gender Male (M) or Female(F). " << std::endl)
#define PHONE_ERROR   (std::cerr << "Please enter a valid phone number. " << std::endl)
#define NAME_ERROR    (std::cerr << "Please enter a correct name. " << std::endl)
#define ADDRESS_ERROR (std::cerr << "Please enter a correct address. " << std::endl)
#define OCCUP_ERROR   (std::cerr << "Please enter a correct occupation. " << std::endl)
#define CTY_ERROR     (std::cerr << "Please enter a correct City. " << std::endl)
#define ST_ERROR      (std::cerr << "Please enter a correct State. " << std::endl)
#define CY_ERROR      (std::cerr << "Please enter a correct Country. " << std::endl)


enum { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY } day;
enum { January, February, March, April, May, June, July,
       August, September, October, November, December} month;

struct PatientStruct
{
    std::string firstName;
    std::string lastName;
    std::string gender;
    std::string email;
    std::string phoneNumber;
    std::string dob;
    std::string occupation;

    struct PatResAddress {
        std::string street;
        std::string aptNumber;
        std::string city;
        std::string state;
        std::string zipCode;
        std::string country;
    } residentAddress;
    struct  PatMailAddress {
        std::string street;
        std::string aptNumber;
        std::string city;
        std::string state;
        std::string zipCode;
    } mailAddress;
};


class Patient
{
public:
    Patient();
    Patient                     (int argc, char** argv);
    virtual                     ~Patient();
    PatientStruct               patInfo;
    std::string                 getFirstName();
    std::string                 getLastName();
    std::string                 getGender();
    std::string                 theLocalTime()          const;
    std::string                 todaysDate()            const;
    std::string                 todaysDateAmerican()    const;
    bool                        _mailingAndResidAddSame=true;
    bool                        verbose                =false;
    std::vector<int>            localTimeVec;
    StringList                  stringSplit (std::string s, std::string d);
    uint                        getAge();
    void                        createPatientInfo();
    void                        populatePatientInfoFromCmdLine();
    void                        bioDatalistLoop(std::function<bool (char *)> query, std::string qA, char* var, uint SIZE);
    void resetId                (uint&);
    bool isAlphaNumeric         (std::string);
    bool containsAChar          (std::string);
    bool dateRangeIsGood        (std::string, int&, int&, int&);
    bool isANumber              (char);
    void setAge                 (uint);
    void setFirstName           (std::string);
    void setLastName            (std::string);
    void setPatientData         (std::string, std::string, std::string, std::string, std::string, std::string, std::string o);
    void setMailingAddress      (std::string, std::string, std::string,std::string, std::string);
    void setResidentialAddress  (std::string, std::string, std::string, std::string, std::string, std::string, bool);



protected:
    bool addressIsEmpty();
    bool americanDate            = false;
    //bool isANumber               (char);
    bool containsANumber         (std::string);
    bool appellationIsRight      (std::string);
    bool middleNameIsRight       (std::string);
    bool genderIsRight           (std::string);
    bool emailFormatIsRight      (std::string);
    bool phoneNumberFormatIsRight(std::string);
    bool DOBFormatIsRight        (std::string);
    bool DateFormatIsRight       (std::string);
    bool streetAddressIsRight    (std::string);
    bool countryIsRight          (const char*);
    bool stateIsRight            (const char*);
    bool zipCodeIsRight          (std::string);
    bool noUnconvChars           (std::string);
    bool noUnconvCharNames       (std::string);
    bool twoConsecutiveChars     (std::string);
    bool twoConsecutiveAlphaChars(std::string);
    bool isACharA                 (char);

private:
    uint                         _age;
    bool                         _cmdInput=false;
    std::string                  _firstName;
    std::string                  _lastName;
    std::string                  _gender;
    std::string                  _email;
    std::string                  _phoneNumber;
    std::string                  _cellPhoneNumber;
    std::string                  _dob;
    std::string                  _occupation;
    std::vector<int>             _dobVec;
    tm*                          _todaysDate;
    bool                         _welcome;
    void                         computeAge();
    void                         welcomeMessage();
    void                         updatePatientInfo();
    void removeWhiteSpace        (std::string&);
    bool getDateDigits           (std::string&, std::string&,
                                  std::string&, std::string);

};

#endif // PATIENT_H
