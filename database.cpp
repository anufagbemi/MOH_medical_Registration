#include "database.h"


DataBase::~DataBase()
{
}

DataBase::DataBase(const char *filename)
{

    sqlite3 *db;

    _rc = sqlite3_open
        (
            filename,
            &db
            );

    if( _rc )
    {
        fprintf(stderr, "Can't open DataBase: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
}

int DataBase::createTable(const char* s)
{
    sqlite3 *db;

    char *errMsg = 0;
    std::string sql = "CREATE TABLE IF NOT EXISTS HOSPITAL_APPOINTMENT("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "NAME          CHAR(50) , "
                      "LNAME         CHAR(50) , "
                      "AGE           INT  NOT NULL, "
                      "GENDER        CHAR(10)  , "
                      "EMAIL         CHAR(50)  , "
                      "PHONE         CHAR(50)  , "
                      "OCCUPATION    CHAR(50)  , "
                      "RES_ADDRESS  CHAR(100) , "
                      "MAIL_ADDRESS  CHAR(100), "
                      "DATE          CHAR(10) , "
                      "FIRST_TIME_VISIT  CHAR(5), "
                      "PREVIOUS_LAB_VISIT  CHAR(10), "
                      "LAB_TESTS_DONE         CHAR(100), "
                      "REASON_FOR_CURRENT_VISIT   CHAR(100)  , "
                      "ALLERGIES         CHAR(100), "
                      "PREGNANT            CHAR(5), "
                      "ANEMIC            CHAR(5), "
                      "ARTHRITIS            CHAR(5), "
                      "ASTHMATIC              CHAR(5), "
                      "DIABETIC              CHAR(5), "
                      "HIGH_BLOOD_PRESSURE    CHAR(5), "
                      "HIGH_CHOLESTEROL    CHAR(5), "
                      "FREQUENT_ALCOHOLIC    CHAR(5), "
                      "FREQUENT_SMOKER       CHAR(5), "
                      "DAY_START             CHAR(20)  , "
                      "APPOINTMENT_INTERVAL  INT  NOT NULL, "
                      "LUNCH_ID              INT  NOT NULL, "
                      "TOTAL_SLOTS           INT  NOT NULL, "
                      "PATIENT_APPOINTMENT  CHAR(20)  , "
                      "DOCTORS_DIAGNOSIS    CHAR(100) );";

    try
    {
        int rcd = 0;
        rcd = sqlite3_open(s, &db);

        const char* str = "SQL table";
        rcd = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
        if (rcd != SQLITE_OK)
        {
            fprintf(stderr, "Table SQL error: %s\n", errMsg);
            sqlite3_free(errMsg);
        }
        else
        {
            printf("Table created Successfully. \n");
        }

        sqlite3_close(db);
    }

    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 0;
}

void DataBase::createAddress(Patient *pat, std::string& address, bool residential)
{
    if (residential)
    {
        address = pat->patInfo.residentAddress.street;
        address += " " + pat->patInfo.residentAddress.aptNumber  +
                   " " + pat->patInfo.residentAddress.city    +
                   " " + pat->patInfo.residentAddress.state   +
                   " " + pat->patInfo.residentAddress.zipCode +
                   " " + pat->patInfo.residentAddress.country;
    }
    else
    {
        address = pat->patInfo.mailAddress.street;
        address += " " + pat->patInfo.mailAddress.aptNumber     +
                   " " + pat->patInfo.mailAddress.city      +
                   " " + pat->patInfo.mailAddress.state     +
                   " " + pat->patInfo.mailAddress.zipCode   +
                   " " + pat->patInfo.residentAddress.country;
    }
}

int DataBase::insertPatientInfo(const char* s, Patient* pat, PatientHistory *patHist,
                                        std::string startWorkTime, uint interval, uint lunchId, uint totalSlots, std::string appointmentTime)  // entire row
{
    sqlite3* db;
    char* errMsg=0;


    std::string resAddress, mailAddress; bool residential=true;
    createAddress(pat, resAddress, residential);

    residential=false;
    createAddress(pat, mailAddress, residential);


    std::ostringstream allData;
    allData << "INSERT INTO HOSPITAL_APPOINTMENT ("
               "NAME, "
               "LNAME, "
               "AGE, "
               "GENDER, "
               "EMAIL,  "
               "PHONE,  "
               "OCCUPATION, "
               "RES_ADDRESS, "
               "MAIL_ADDRESS, "
               "DATE,         "
               "FIRST_TIME_VISIT, "
               "PREVIOUS_LAB_VISIT, "
               "LAB_TESTS_DONE,      "
               "REASON_FOR_CURRENT_VISIT, "
               "ALLERGIES, "
               "PREGNANT, "
               "ANEMIC, "
               "ARTHRITIS, "
               "ASTHMATIC, "
               "DIABETIC, "
               "HIGH_BLOOD_PRESSURE, "
               "HIGH_CHOLESTEROL, "
               "FREQUENT_ALCOHOLIC, "
               "FREQUENT_SMOKER , "
               "DAY_START, "
               "APPOINTMENT_INTERVAL,  "
               "LUNCH_ID,       "
               "TOTAL_SLOTS,       "
               "PATIENT_APPOINTMENT) VALUES('"

            <<  pat->patInfo.firstName                  << "','"
            <<  pat->patInfo.lastName                   << "',"
            << std::to_string(pat->getAge())            << ",'"
            << pat->patInfo.gender                      << "','"
            << pat->patInfo.email                       << "','"
            << pat->patInfo.phoneNumber                 << "','"
            << pat->patInfo.occupation                  << "','"
            << resAddress                               << "','"
            << mailAddress                              << "','"
            << pat->todaysDate()                        << "','"
            << patHist->patHistData.firstTimeVisit      << "','"
            << patHist->previousLabAppointment()        << "','"
            << patHist->getPreviousLabTestsDone()       << "','"
            << patHist->getReasonForVisit()             << "','"
            << patHist->getAllergyList()                << "','"
            << patHist->patHistData.pregnant            << "','"
            << patHist->patHistData.anemia              << "','"
            << patHist->patHistData.arthritis           << "','"
            << patHist->patHistData.asthma              << "','"
            << patHist->patHistData.diabetes            << "','"
            << patHist->patHistData.hBloodPressure      << "','"
            << patHist->patHistData.hCholesterol        << "','"
            << patHist->patHistData.drinksFrequently    << "','"
            << patHist->patHistData.smokesFrequently    << "','"
            << startWorkTime                            << "',"
            << std::to_string(interval)                 << ","
            << std::to_string(lunchId)                  << ","
            << std::to_string(totalSlots)               << ",'"
            << appointmentTime << "');"
        ;

    const std::string sql(allData.str());
    //std::cout << sql << std::endl;

    int rcd = sqlite3_open(s, &db);

    rcd = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);
    if (rcd != SQLITE_OK)
    {
        std::cerr << "Error in insertData function. Data could not be saved." << std::endl;
        sqlite3_free(errMsg);
    }
    else
    {
        std::cout << "Records inserted Successfully!" << std::endl;
    }
    return 0;
}

int DataBase::getSQLData(const char* s, StringList& date,
                                 StringList& dayStart, std::vector<uint>& totalSlots,
                                 std::vector<uint>& meetingIncrement, std::vector<uint>& meetingId,
                                 StringList& patientAppointments)
{

    sqlite3* db;
    sqlite3_stmt* stmt;
    char* errMsg=0;

    std::string sql = "SELECT * FROM HOSPITAL_APPOINTMENT;";

    int rcd = sqlite3_open(s, &db);

    rcd = sqlite3_exec(db, sql.c_str(), callback, NULL, &errMsg);


    if (rcd != SQLITE_OK)
    {
        fprintf(stderr, "Error in selectData function. \n");
        sqlite3_free(errMsg);
    }
    else
    {
        printf("Records loaded Successfully!\n");
    }

    // load the data to variables
    std::string sql2 = "SELECT "
                       "DATE,         "
                       "DAY_START, "
                       "PATIENT_APPOINTMENT, "
                       "APPOINTMENT_INTERVAL,  "
                       "LUNCH_ID,       "
                       "TOTAL_SLOTS       "
                       "FROM HOSPITAL_APPOINTMENT";

    int ref;
    if (SQLITE_OK != (ref =sqlite3_prepare_v2(db, sql2.c_str(), -1,  &stmt, NULL)))
    {
        printf("Failed to prepare insert: %d, %s\n", ref, sqlite3_errmsg(db));
    }

    int i=0;
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {

        date.push_back(std::string(reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 0))));

        dayStart.push_back(std::string(reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 1))));

        patientAppointments.push_back(std::string(reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 2))));

        meetingIncrement.push_back(sqlite3_column_int(stmt, 3));

        meetingId.push_back(sqlite3_column_int(stmt, 4));

        totalSlots.push_back(sqlite3_column_int(stmt, 5));

        std::cout << "Date: " << date[i] << " Start Time: " << dayStart[i]
                  << ", Increm: " << meetingIncrement[i] << " ID: " << meetingId[i]
                  << ", Num of slots for day: " << totalSlots[i]
                  << ", Appointment Time: " << patientAppointments[i] << std::endl;
        i++;
    }
    return 0;
}


int DataBase::sqlDataBaseExec(sqlite3* db, int rc, const char* str, std::string sql, char* zErrMsg)
{

    if( rc )
    {
        fprintf(stderr, "Can't open DataBase2: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error in %s\n", str);
        sqlite3_free(zErrMsg);
    }
    else
    {
        printf("%s run successfully", str);
    }

    return rc;
}


int DataBase::updatePatientInfo(const char* s)
{

    sqlite3* db;
    char* zErrMsg=0;

    std::string sql("UPDATE HOSPITAL_APPOINTMENT SET DOCTORS_DIAGNOSIS = 'Doing well' WHERE LNAME = 'Josephs'");

    int rcd = sqlite3_open(s, &db);

    rcd = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rcd != SQLITE_OK) {
        std::cerr << "Error in updateData function." << std::endl;
            sqlite3_free(zErrMsg);
    }
    else
        std::cout << "Records updated Successfully!" << std::endl;

    return 0;
}

int DataBase::selectDataBaseInfo(const char* s)
{
    sqlite3* db;
    char* errMsg=0;

    std::string sql = "SELECT * FROM HOSPITAL_APPOINTMENT;";
    int rcd = sqlite3_open(s, &db);

    rcd = sqlite3_exec(db, sql.c_str(), callback, NULL, &errMsg);

    if (rcd != SQLITE_OK)
    {
        fprintf(stderr, "Error in selectData function. \n");
        sqlite3_free(errMsg);
    }
    else
    {
        printf("Records gotten Successfully!\n");
    }

    return 0;
}

int DataBase::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
