/* 
 * File:   Database.cpp
 * Author: tuannguyen
 * 
 * Created on 11 August 2015, 4:07 PM
 */

#include "Database.h"


const SQLString  Database::hostname = "127.0.0.1";
const SQLString Database::password = "E4XACjVH";
const SQLString Database::schema = "student32";
const SQLString Database::username = "student32";

Database::Database() {
    driver = NULL;
    dbConnection = NULL;
}

Database::~Database() {
    if (dbConnection != NULL)
    {
        dbConnection->close();
        delete dbConnection;
    }
}

Connection * Database::getConnection() throw (SQLException)
{
    if (dbConnection == NULL)
    {
        driver = get_driver_instance();
        if (driver != NULL)
        {
            dbConnection = driver ->connect(hostname, username, password);
            dbConnection ->setSchema(schema);
        }
        else
        {
            throw SQLException("Unable to find a driver");
        }
    }
    return dbConnection;
}
