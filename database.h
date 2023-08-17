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

#ifndef DataBase_H
#define DataBase_H

#include <stdio.h>
#include <sqlite3.h>
//#include <sqlite3ext.h>
#include "patientHistory.h"

class DataBase
{
public:
    DataBase();
    DataBase(const char *filename);
    ~DataBase();

    static int callback(void*, int, char**, char **);
    static int createTable(const char*);
    static int insertPatientInfo(const char*, Patient*, PatientHistory*, std::string, uint, uint, uint, std::string);
    static int updatePatientInfo(const char*);
    static int selectDataBaseInfo(const char*);   // get the data
    static int sqlDataBaseExec(sqlite3*, int, const char*, std::string, char*);
    static int getSQLData(const char* s, StringList&, StringList&, std::vector<uint>&,
                          std::vector<uint>&, std::vector<uint>&, StringList&);
    static void createAddress(Patient *pat, std::string& address, bool residential);

private:
    int _rc=0;
};

#endif // DataBase2_H
