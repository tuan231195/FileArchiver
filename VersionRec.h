/* 
 * File:   VersionRec.h
 * Author: tuannguyen
 *
 * Created on 13 August 2015, 6:45 PM
 */

#ifndef VERSIONREC_H
#define	VERSIONREC_H
#include <vector>
#include <stdexcept>
#include <time.h>
#include "Database.h"
#include "cppconn/prepared_statement.h"
using namespace std;


class Block
{
public:
    char *data;
    int length;
    int blocknum;
    int hash;
    Block();
    ~Block();
};



class VersionRec {
public:
    VersionRec(string);//used to create a new version record;
    VersionRec(int, string);//used to retrieve a particular version record from the database
    ~VersionRec();
    bool getDataFromDatabase() throw (SQLException);
    void saveDataToDatabase() throw (SQLException);
    void addBlock(Block*);
    void setHash(int);
    void setLength(long) throw (runtime_error);
    void setVersionNum(int);
    void setModifyTime(struct timespec);
    struct timespec getModifyTime() const
    {
        return modifyTime;
    };
    long getLength() const
    {
        return length;
    }
    int getId() const
    {
        return id;
    }
    int getVersionNum() const
    {
        return versionNum;
    }
    int getHash() const
    {
        return hash;
    }
    vector<Block*>* getBlocks()
    {
        return &blocks;
    }
private:
    string fileRef;
    Database database;
    void init();
    int id;
    int versionNum;
    long length;
    struct timespec modifyTime;
    int hash;
    vector<Block*> blocks;
};


#endif	/* VERSIONREC_H */

