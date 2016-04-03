/* 
 * File:   Database.h
 * Author: tuannguyen
 *
 * Created on 11 August 2015, 4:07 PM
 */

#ifndef DATABASE_H
#define	DATABASE_H

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/exception.h>
using namespace sql;

class Database {
public:
    Database();
    ~Database();
    Connection * getConnection() throw (SQLException);
private:
    Connection *dbConnection;
    Driver *driver;
    static const SQLString hostname;
    static const SQLString username;
    static const SQLString password;
    static const SQLString schema;
    
    
    Database(const Database& orig);
    Database& operator=(const Database&);
};

#endif	/* DATABASE_H */

