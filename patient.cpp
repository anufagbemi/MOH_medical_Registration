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

#include "patient.h"
#include <cstring>
#include <iostream>
#include <sstream>


Patient::Patient():
    verbose(false),
    americanDate(false)
{}

void Patient::bioDatalistLoop(std::function<bool(char*)> query, std::string qA, char* var, uint SIZE)
{
    uint i=0;
    while (i<I_MAX)
    {
        std::cout << qA;
        std::cin.getline (var, SIZE);

        if (query(var))
        {
            resetId(i);
            break;
        }
        else
        {
            i++;
        }
    }
}

void Patient::populatePatientInfoFromCmdLine()
{
    welcomeMessage();

    uint i=0;
    char f_name[MAX_NAME_LEN],    l_name[MAX_NAME_LEN],  m_name[MAX_NAME_LEN];
    char r_Country[MAX_ZIP_LEN],  r_S1[MAX_ADDRESS_LEN], r_S2[MAX_ZIP_LEN], r_City[MAX_ZIP_LEN], r_St[MAX_ZIP_LEN], r_Zip[MAX_ZIP_LEN];
    char m_Country[MAX_ZIP_LEN],  m_S1[MAX_ADDRESS_LEN], m_S2[MAX_ZIP_LEN], m_City[MAX_ZIP_LEN], m_St[MAX_ZIP_LEN], m_Zip[MAX_ZIP_LEN];

    char phone[MAX_PHONE_LEN], gender[MAX_GENDER_LEN], email[MAX_PHONE_LEN], dob[MAX_DOB_LEN], occupation[MAX_NAME_LEN];
    char dummyVar[MAX_GENDER_LEN];


    std::cout << ("Please fill out the following information below.\n\n");

    bioDatalistLoop([&](char* var) { return appellationIsRight(var); },
                    "First Name: ", f_name, MAX_NAME_LEN);

    bioDatalistLoop([&](char* var) { return appellationIsRight(var); },
                    "Last Name (Surname): ", l_name, MAX_NAME_LEN);

    while(i<I_MAX)
    {
        std::cout << "Middle Name or initial (optional): " ;
        std::cin.getline(m_name, MAX_NAME_LEN);
        std::string tmp=m_name;

        if(tmp.empty())
            break;
        else
        {
            if(middleNameIsRight(m_name))
            {
                resetId(i);
                break;
            }
            else
            {
                i++;
            }
        }
    }
    bioDatalistLoop([&](char* var) { return appellationIsRight(var); },
                    "Country of residence: ", r_Country, MAX_ADDRESS_LEN);

    bioDatalistLoop([&](char* var) { return streetAddressIsRight(var); },
                    "Residential Address (Street): ", r_S1, MAX_ADDRESS_LEN);

    while(i<I_MAX)
    {
        std::cout << "Street 2 (optional): ";
        std::cin.getline (r_S2, MAX_ADDRESS_LEN);
        std::string tmp=r_S2;

        if(tmp.empty())
            break;
        else
        {
            if(noUnconvChars(tmp))
            {
                resetId(i);
                break;
            }
            else
            {
                ADDRESS_ERROR;
                i++;
            }
        }
    }
    bioDatalistLoop([&] (char* var) { return appellationIsRight(var); },
                        "City: ", r_City, MAX_ADDRESS_LEN);

    bioDatalistLoop([&](char* var) { return appellationIsRight(var); },
                    "State: ", r_St, MAX_ZIP_LEN);

    bioDatalistLoop([&](char* var) { return zipCodeIsRight(var); },
                    "Zipcode or PMB (e.g. 82300): ", r_Zip, MAX_ZIP_LEN);

    while(i<I_MAX)
    {
        std::cout << "Above address same as mailing Address (Y/N)?: ";
        std::cin.getline (dummyVar, MAX_GENDER_LEN);

        if((strcasecmp(dummyVar, "Y")==0) || (strcasecmp(dummyVar, "yes")==0))
        {
            for (uint i=0; i<MAX_ADDRESS_LEN; i++)
            {
                m_S1[i]   = r_S1[i];
                m_S2[i]   = r_S2[i];
            }
            for (uint i=0; i<MAX_NAME_LEN; i++)
            {
                m_City[i]    = r_City[i];
                m_St[i]      = r_St[i];
                m_Zip[i]     = r_Zip[i];
                m_Country[i] = r_Country[i];
            }
            break;
        }
        else if((strcasecmp(dummyVar, "N")==0) || (strcasecmp(dummyVar, "no")==0))
        {
            _mailingAndResidAddSame=false;

            bioDatalistLoop([&](char* var) { return streetAddressIsRight(var); },
                            "Mailing Address (Street): ", m_S1, MAX_ADDRESS_LEN);

            while(i<I_MAX)
            {
                std::cout << "Street 2 (optional): ";
                std::cin.getline (m_S2, MAX_ADDRESS_LEN);
                std::string tmp=m_S2;

                if(tmp.empty())
                    break;
                else
                {
                    if(noUnconvChars(tmp))
                    {
                        resetId(i);
                        break;
                    }
                    else
                    {
                        ADDRESS_ERROR;
                        i++;
                    }
                }
            }
            bioDatalistLoop([&] (char* var) { return appellationIsRight(var); },
                            "City: ", m_City, MAX_ADDRESS_LEN);

            bioDatalistLoop([&](char* var) { return appellationIsRight(var); },
                            "State: ", m_St, MAX_ZIP_LEN);

            bioDatalistLoop([&](char* var) { return zipCodeIsRight(var); },
                            "Zipcode or PMB (e.g. 82300): ", m_Zip, MAX_ZIP_LEN);
            break;
        }
        else
        {
            std::cerr << "Please enter a correct option. ";
            i++;
        }
    }

    bioDatalistLoop([&] (char* var) { return appellationIsRight(var); },
                    "Occupation: ", occupation, MAX_ADDRESS_LEN);

    bioDatalistLoop([&](char* var) { return genderIsRight(var); },
                    "Gender (Male or Female): ", gender, MAX_GENDER_LEN);

    bioDatalistLoop([&](char* var) { return DOBFormatIsRight(var); },
                    "Date of birth (DD/MM/YYYY): ", dob, MAX_DOB_LEN);

    bioDatalistLoop([&](char* var) { return phoneNumberFormatIsRight(var); },
                    "Phone Number (with Area code, e.g +1 123456789): ", phone, MAX_PHONE_LEN);

    bioDatalistLoop([&](char* var) { return emailFormatIsRight(var); },
                    "Email: ", email, MAX_NAME_LEN);

    std::string tmp;
    std::stringstream X(f_name);

    int id=0; bool bothNames=false;
    if (bothNames)
    {
        while (getline(X, tmp, ' '))
        {
            if(id==0)
                _firstName = tmp;
            else if (id==1)
                _lastName = tmp;
            else
                break;
            id++;
        }
    }

    _firstName = f_name;
    _lastName  = l_name;
    _occupation = occupation;

    // Update address
    patInfo.residentAddress.street    = r_S1;
    patInfo.residentAddress.aptNumber = r_S2;
    patInfo.residentAddress.city      = r_City;
    patInfo.residentAddress.state     = r_St;
    patInfo.residentAddress.zipCode   = r_Zip;
    patInfo.residentAddress.country   = r_Country;

    patInfo.mailAddress.street        = m_S1;
    patInfo.mailAddress.aptNumber     = m_S2;
    patInfo.mailAddress.city          = m_City;
    patInfo.mailAddress.state         = m_St;
    patInfo.mailAddress.zipCode       = m_Zip;

    _cmdInput=true;

    this->updatePatientInfo();
}

void Patient::welcomeMessage()
{
    printf("%s", "==========================================================\n");
    printf("%s", "======        Welcome to MOH Clinic           ======\n");
    printf("%s", "==========================================================\n");
    _welcome=true;
}

void Patient::setPatientData(std::string f, std::string l, std::string g, std::string e, std::string p,
                             std::string d, std::string o)
{

    if(!_welcome) welcomeMessage();

    _firstName   = f;
    _lastName    = l;
    _gender      = g;
    _email       = e;
    _phoneNumber = p;
    _dob         = d;
    _occupation  = o;

    // Put the information into a struct
    this->updatePatientInfo();
}

void Patient::updatePatientInfo()
{
    patInfo.firstName   = _firstName;
    patInfo.lastName    = _lastName;
    patInfo.gender      = _gender;
    patInfo.email       = _email;
    patInfo.phoneNumber = _phoneNumber;
    patInfo.dob         = _dob;
    patInfo.occupation  = _occupation;
}

// Get today's date
std::string getUTCTime_Date()
{
    // current date/time based on current system
   time_t now = time(0);

   // convert now to std::string form
   char* dt = ctime(&now);

   // convert now to tm struct for UTC
   tm *gmtm = gmtime(&now);
   dt = asctime(gmtm);

   return dt;
}

std::string Patient::theLocalTime() const
{
    std::string hour   = std::to_string(_todaysDate->tm_hour);
    std::string minute = std::to_string(_todaysDate->tm_min);
    std::string sec    = std::to_string(_todaysDate->tm_sec);

    std::cout << "Time: "<< hour << ":";
    std::cout << minute << ":";
    std::cout << sec << std::endl;

    return hour+":"+minute+":"+sec;
};

std::string Patient::todaysDate() const
{
    // Get today's date 05/19/2023
    time_t now = time(0);

    tm *ltm = localtime(&now);

    std::string date;

    date += std::to_string(ltm->tm_mday)+"/"+std::to_string(1 + ltm->tm_mon)+
            "/"+std::to_string(YEAR_REF + ltm->tm_year);

    return date;
}

std::string Patient::todaysDateAmerican() const
{
    // Get today's date 05/19/2023
    time_t now = time(0);

    tm *ltm = localtime(&now);

    std::string date;

    date += std::to_string(1 + ltm->tm_mon)+"/"+std::to_string(ltm->tm_mday)+
            "/"+std::to_string(YEAR_REF + ltm->tm_year);

    return date;
}

void Patient::computeAge()
{

    time_t now = time(0);

    tm *ltm = localtime(&now);

   uint calendarYear  =  YEAR_REF + ltm->tm_year;
   uint calendarMonth =  1 + ltm->tm_mon;
   uint calendarDay   =  ltm->tm_mday;

   _todaysDate = ltm;
   _todaysDate->tm_hour = 2+ltm->tm_hour;
   _todaysDate->tm_min  = 30+ltm->tm_min;
   _todaysDate->tm_sec  = ltm->tm_sec;

   std::vector<std::string> dobVec = stringSplit(_dob, "/");

   uint dob_day   = stoi(dobVec[0]);
   uint dob_month = stoi(dobVec[1]);
   uint dob_yr    = stoi(dobVec[2]);

//   if (americanDate)
//   {
//        int tmp=dob_day;
//        dob_day   = dob_month;
//        dob_month = tmp;
//   }

    int yearDiff = calendarYear - dob_yr;
    // check month
    if (calendarMonth < dob_month)
    {
        _age = yearDiff-1;
    }
    else if (calendarMonth == dob_month)
    {
        if (calendarDay<dob_day)
            _age = yearDiff-1;
        else if (calendarDay==dob_day)
            _age = yearDiff;
        else
            _age = yearDiff;
    }
    else
    {
        _age = yearDiff;
    }
}

void Patient::createPatientInfo()
{
    if (!_cmdInput) {

        if(!streetAddressIsRight(patInfo.residentAddress.street)|| !appellationIsRight(patInfo.residentAddress.city)    ||
            !appellationIsRight(patInfo.residentAddress.state)  || !appellationIsRight(patInfo.residentAddress.country) ||
            !zipCodeIsRight(patInfo.residentAddress.zipCode))
        {
            std::cerr << std::endl << "Enter a correct adddress, City, State, Country or zip-code.";
            exit(EXIT_FAILURE);
        }
        if( !appellationIsRight(_firstName) || !appellationIsRight(_lastName))
        {
            std::cerr << "Enter a valid name."  << std::endl << std::endl;
            exit(EXIT_FAILURE);
        }
        if( !appellationIsRight(_firstName) || !appellationIsRight(_lastName)       || !genderIsRight(_gender) ||
            !emailFormatIsRight(_email) ||  !phoneNumberFormatIsRight(_phoneNumber) || !DOBFormatIsRight(_dob))
        {
            exit(EXIT_FAILURE);
        }
    }
    assert(!_firstName.empty()&& !_lastName.empty() && !_gender.empty() &&
           !_email.empty() && !_dob.empty() && !_phoneNumber.empty() &&
           !addressIsEmpty());

    this->computeAge();

    std::cout << "+++++++++ Patient's Summary ++++++++" << std::endl;
    std::cout << "Name: "    << _firstName   << " " << _lastName << std::endl;
    std::cout << "Gender: "  << _gender      << std::endl;
    std::cout << "Email: "   << _email       << std::endl;
    std::cout << "Phone: "   << _phoneNumber << std::endl;
    std::cout << "Age: "     << _age         << std::endl;
    std::cout << "Address: " << patInfo.residentAddress.street     << " "
                             << patInfo.residentAddress.aptNumber  << ", "
                             << patInfo.residentAddress.city       << ", "
                             << patInfo.residentAddress.state      << ", "
                             << patInfo.residentAddress.country    << std::endl;
}

bool Patient::appellationIsRight(std::string name)
{

    if(name.size()==0)
    {
        INPUT_ERROR;
        return false;
    }

    if( containsANumber(name) || !twoConsecutiveChars(name) ||
     !noUnconvCharNames(name))
    {
        INPUT_ERROR;
        return false;
    }

    return true;
}

// Check the format
bool Patient::middleNameIsRight(std::string name)
{

    if(containsANumber(name) || !noUnconvCharNames(name))
    {
        INPUT_ERROR;
        return false;
    }

    if(name.size()==1 && name[0]=='-')
    {
        INPUT_ERROR;
        return false;
    }

    return true;
}

bool Patient::genderIsRight(std::string gender)
{

    const char *opt1 = "M";
    const char *opt2 = "F";
    const char *opt3 = "Male";
    const char *opt4 = "Female";

    if ((strcasecmp(gender.c_str(), opt1) != 0) &&
        (strcasecmp(gender.c_str(), opt2) != 0) &&
        (strcasecmp(gender.c_str(), opt3) != 0) &&
        (strcasecmp(gender.c_str(), opt4) != 0)) {

        GEND_ERROR;
        return false;
    }

    _gender = gender;

    return true;
}

bool Patient::getDateDigits(std::string& day,
                            std::string& month,
                            std::string& year, std::string date)
{

    // British format day   dd/mm/yyyy
    for (int i=0; i< mCOUNT-1; ++i)
    {
        char l = date[i];
        int lTemp = l;

        if (i==0 && lTemp > 0)
            day += l;

        if (i > 0)
            day += l;

        if (i == 1) {
            if ((i==0 && lTemp == 0) && (i==1 && lTemp == 0)) {
                DATE_ERROR;
                return false;
            }
        }
    }

    // British format month
    for (int i=mCOUNT; i< yCOUNT-1; i++)
    {
        char l = date[i];
        int lTemp = l;

        if (i==mCOUNT && lTemp > 0)
            month += l;

        if (i > mCOUNT)
            month += l;

        if (i == yCOUNT-2) {
            if ((i==mCOUNT && lTemp == 0) && (i==yCOUNT-2 && lTemp == 0)) {
                DATE_ERROR;
                return false;
            }
        }
    }

    // British format year
    for (uint i=yCOUNT; i< date.length(); ++i)
    {
        char l = date[i];
        year += l;
    }

    if (year.size() != 4)
    {
        std::cerr << "Please enter year in YYYY format. "  << std::endl;
        return false;
    }

    return true;
}

bool Patient::dateRangeIsGood(std::string date, int& day_i, int& month_i, int& year_i)
{

    std::string day, month, year;
    if(!getDateDigits(day, month, year, date))
        return false;

    day_i = stoi(day);
    month_i = stoi(month);
    year_i = stoi(year);

//    if(americanDate)
//    {
//        int tmp_i = month_i;
//        month_i = day_i;
//        day_i = tmp_i;
//    }

    StringList tmp = stringSplit(todaysDate(), "/");
    auto day_Td=tmp[0], month_Td=tmp[1];
    auto year_Td =stoi(todaysDate().substr(todaysDate().find_last_of("/")+1));

    if ( (day_i  < 1    || day_i   > 31      )     ||
        (month_i < 1    || month_i > 12      )     ||
        (year_i  < 1900 || year_i  > year_Td))
    {
        DATE_ERROR;
        return false;
    }

    if(year_Td == year_i)
    {
        if ((day_i >= stoi(day_Td)) && (month_i >= stoi(month_Td)))
        {
            DATE_ERROR;
            return false;
        }
    }

    return true;
}

bool Patient::streetAddressIsRight(std::string address)
{
    if( address.empty() || !twoConsecutiveChars(address) || !noUnconvChars(address) ||
        !containsAChar(address) ||!twoConsecutiveAlphaChars(address))
    {
        ADDRESS_ERROR;
        return false;
    }
    return true;
}

bool Patient::DOBFormatIsRight(std::string dob)
{

    if(dob.size()==0)
    {
        DOB_ERROR;
        return false;
    }

    int day_i;
    int month_i;
    int year_i;

    if(!dateRangeIsGood(dob, day_i, month_i, year_i))
        return false;

    if (verbose)
    {
        std::cout << "day of birth: "   << day_i   << std::endl;
        std::cout << "month of birth: " << month_i << std::endl;
        std::cout << "year of birth: "  << year_i  << std::endl;
        std::cout << "Age: "  << getAge()  << std::endl;
    }

    _dobVec.push_back(day_i);
    _dobVec.push_back(month_i);
    _dobVec.push_back(year_i);

    _dob = dob;
    return true;
}

bool Patient::DateFormatIsRight(std::string date)
{

    if(date.size()==0)
    {
        DATE_ERROR;
        return false;
    }

    int d_, m_, y_;

    if(!dateRangeIsGood(date, d_, m_, y_))
        return false;

    return true;
}

StringList Patient::stringSplit(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

bool Patient::emailFormatIsRight(std::string email)
{

    /* EMAIL RULES: https://en.wikipedia.org/wiki/Email_address
    If unquoted, it may use any of these ASCII characters:uppercase and lowercase
    Latin letters A to Z and a to z digits 0 to 9, printable characters !#$%&'*+-/=?^_`{|}~
    dot ., provided that it is not the first or last character and provided also
    that it does not appear consecutively (e.g., John..Doe@example.com is not allowed).[7]

    If quoted, it may contain Space, Horizontal Tab (HT),any ASCII graphic except Backslash
    and Quote and a quoted-pair consisting of a Backslash followed by HT,Space or
    any ASCII graphic; it may also be split between lines anywhere that HT or Space appears.

    In contrast to unquoted local-parts, the addresses ".John.Doe"@example.com,
    "John.Doe."@example.com and "John..Doe"@example.com are allowed.
    */

    if(email.size()==0)
    {
        EMAIL_ERROR;
        return false;
    }

    std::string userId;
    std::string domain;


    if (stringSplit(email, "@").size() != 2)
    {
        EMAIL_ERROR;
        return false;
    }

    userId = stringSplit(email, "@")[0];
    domain = stringSplit(email, "@")[1];


    auto found = domain.find(".");
    if (found == std::string::npos)
    {
        EMAIL_ERROR;
        return false;
    }

    bool quotedEmail=false;
    if(domain[0]=='"') quotedEmail=true;

    std::string email_ext= email.substr(email.find_last_of(".")+1);

    // if no domain ext
    if (email_ext.empty())
    {
        EMAIL_ERROR;
        return false;
    }

    if (!quotedEmail)
    {
        for (uint i=0; i< email.size(); i++)  // Check if dot is first or last character, or is consecutive
        {
            if ((email[0] == '.') || (email[email.size()-1] == '.'))
            {
                EMAIL_ERROR;
                return false;
            }

            // if there are consecutive periods
            if (i != email.size()-1 )
            {
                if ((email[i] == '.') && (email[i+1] == '.'))
                {
                    EMAIL_ERROR;
                    return false;
                }
            }
        }

        if (domain[0] == ' ') // if theres space just after @ symbol
        {
            EMAIL_ERROR;
            return false;
        }

        // if theres space between characters before @ symbol
        uint firstCharId=0;
        for (uint i=0; i< userId.size(); i++)
        {
            if(userId[i] != ' ')
            {
                break;
            }
            firstCharId++;
        }
        if(firstCharId<=userId.size()-2)
        {
            for (uint i=firstCharId; i< userId.size(); i++)
            {
                if(email[i] == ' ')
                {
                    EMAIL_ERROR;
                    return false;
                }
            }
        }

        // if theres space between characters after @ symbol
        uint lastCharId=0;
        for (int i=domain.size()-1; i>-1; i--)
        {
            if(domain[i] != ' ')
            {
                break;
            }
            lastCharId++;
        }
        uint size_ns=domain.size()-lastCharId;
        assert(size_ns>0);

        for (uint i=1; i< size_ns; i++)
        {
            if(domain[i] == ' ')
            {
                EMAIL_ERROR;
                return false;
            }
        }
    }

    _email = email;
    return true;
}

bool Patient::phoneNumberFormatIsRight(std::string phoneNumber)
{
    // Regex to check valid phonenumber.
    const std::regex pattern("^[+]{1}(?:[0-9\\-\\(\\)\\/"
                        "\\.]\\s?)*[0-9]{1}$");

    if (phoneNumber.empty())
    {
        PHONE_ERROR;
        return false;
    }

    if (regex_match(phoneNumber, pattern))
    {
        _phoneNumber = phoneNumber;
        return true;
    }
    else
    {
        PHONE_ERROR;
        return false;
    }
    return true;
}

void Patient::setMailingAddress(std::string m1, std::string m2, std::string city,
                                std::string state, std::string zip)
{
    patInfo.mailAddress.street    = m1;
    patInfo.mailAddress.aptNumber = m2;
    patInfo.mailAddress.city      = city;
    patInfo.mailAddress.state     = state;
    patInfo.mailAddress.zipCode   = zip;
}

void Patient::removeWhiteSpace(std::string& strInput)
{

    std::string tmpInput;

    for (uint i =0; i< strInput.size(); ++i)
    {
        if(strInput[i] != ' ')
        {
            tmpInput.push_back(strInput[i]);
        }
    }
    if (!tmpInput.empty())
    {
        strInput=tmpInput;
    }
    else
    {
        return;
    }
}

bool Patient::isAlphaNumeric(std::string chars)
{

    if(containsAChar(chars) && containsANumber(chars))
        return true;

    return false;
}

bool Patient::twoConsecutiveChars(std::string chars)
{
    bool tmp=false;
    for (uint i=1; i < chars.size()+1; i++)
    {
        if((chars[i-1] != ' ') && (chars[i] != ' ') && (chars[i] != '\0'))
        {
            break;
        }
        if(i==chars.size())
            tmp=true;
    }
    if(tmp)
    {
        return false;
    }
    return true;
}

bool Patient::twoConsecutiveAlphaChars(std::string chars)
{
    bool tmp=false;
    for (uint i=1; i < chars.size()+1; i++)
    {
        if((chars[i-1] != ' ') && (chars[i] != ' ') &&
            (chars[i] != '\0'))
        {
            if(isACharA(chars[i-1]) && isACharA(chars[i]))
                break;
        }
        if(i==chars.size())
            tmp=true;
    }
    if(tmp)
    {
        return false;
    }
    return true;
}

bool Patient::containsAChar(std::string chars)
{
    char alphaArray[]={'a','b','c','d','e','f','g','h','i','j','k', 'l', 'm','n',
                         'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x','y','z' };

    for (uint j=0; j<chars.size(); j++)
    {
        for (uint i=0; i<ALPHA_SIZE; i++)
        {
            if(chars[j]==alphaArray[i])
            {
                return true;
            }
        }
    }
    return false;
}

bool Patient::noUnconvChars(std::string chars)
{
    //char number;
    char uncovStrArray[] ={'!','$','%','&', '*', '+', '\'',  // addresses can have #, -, /
                            '=', '?','^', '_', '`', '{' , '|' , '}' ,'~', ',', '.', '"'};

    for (uint i=0; i<WCHAR_SIZE-2; i++)
    {
        for (uint j=0; j<chars.size(); j++)
        {
            if(chars[j] == uncovStrArray[i])
            {
                return false;
            }
        }
    }

    return true;
}

bool Patient::noUnconvCharNames(std::string chars)
{
    //char number;
    char uncovStrArray[] ={'!','#','$','%','&', '*', '+','/', '\'',  // names can have -.
                            '=', '?','^', '_', '`', '{' , '|' , '}' ,'~', ',', '.', '"'};

    for (uint i=0; i<WCHAR_SIZE; i++)
    {
        for (uint j=0; j<chars.size(); j++)
        {
            if(chars[j] == uncovStrArray[i])
            {
                return false;
            }
        }
    }
    return true;
}

bool Patient::containsANumber(std::string number)
{
    //char number;
    char numArray[] ={'0','1','2', '3','4','5','6','7','8','9'};

    for (uint i=0; i<number.size(); i++)
    {
        for (uint j=0; j<NUM_SIZE; j++)
        {
            if(number[i] == numArray[j])
            {
                return true;
            }
        }
    }
    return false;
}

bool Patient::isACharA(char alphabet)
{
    //char number;
    char alphaArray[] ={'a','b','c','d','e','f','g','h','i','j','k', 'l', 'm','n',
                       'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x','y','z' };

    for (uint j=0; j<ALPHA_SIZE; j++)
    {
        if(alphabet == alphaArray[j])
        {
            return true;
        }
    }
    return false;
}

bool Patient::isANumber(char number)
{
    //char number;
    char numArray[] ={'0','1','2', '3','4','5','6','7','8','9'};

    for (uint j=0; j<NUM_SIZE; j++)
    {
        if(number == numArray[j])
        {
            return true;
        }
    }

    return false;
}

void Patient::setResidentialAddress(std::string r1, std::string r2, std::string city,
                                    std::string state, std::string zip, std::string country, bool resMailSame)
{
    patInfo.residentAddress.street    = r1;
    patInfo.residentAddress.aptNumber = r2;
    patInfo.residentAddress.city      = city;
    patInfo.residentAddress.state     = state;
    patInfo.residentAddress.zipCode   = zip;
    patInfo.residentAddress.country   = country;

    if(resMailSame)
    {
        patInfo.mailAddress.street    = r1;
        patInfo.mailAddress.aptNumber = r2;
        patInfo.mailAddress.city      = city;
        patInfo.mailAddress.state     = state;
        patInfo.mailAddress.zipCode   = zip;
    }
}

bool Patient::addressIsEmpty()
{
    if(!patInfo.residentAddress.street.empty()    ||
        !patInfo.residentAddress.aptNumber.empty() ||
        !patInfo.residentAddress.city.empty()      ||
        !patInfo.residentAddress.state.empty()     ||
        !patInfo.residentAddress.zipCode.empty()   ||
        !patInfo.residentAddress.country.empty()
        )  return false;

    if(!patInfo.mailAddress.street.empty()    ||
        !patInfo.mailAddress.aptNumber.empty() ||
        !patInfo.mailAddress.city.empty()      ||
        !patInfo.mailAddress.state.empty()     ||
        !patInfo.mailAddress.zipCode.empty()
        )  return false;

    return true;
}

void Patient::resetId(uint& i) { i = 0; }


bool Patient::countryIsRight(const char * country)  // Not available in demo version
{
    return true;
}

bool Patient::stateIsRight(const char * state)  // Not available in demo version
{
    return true;
}

bool Patient::zipCodeIsRight(std::string zip) // Not available in demo version
{
    return true;
}

void Patient::setFirstName(std::string f)
{
    _firstName = f;
}

void Patient::setLastName(std::string l)
{
    _lastName = l;
}

std::string Patient::getFirstName()
{
    return _firstName;
}

std::string Patient::getLastName()
{
    return _lastName;
}

uint Patient::getAge()
{
    return _age;
}

std::string Patient::getGender()
{
    return _gender;
}

Patient::~Patient()
{
}
