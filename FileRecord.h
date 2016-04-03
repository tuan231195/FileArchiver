/* 
 * File:   FileRecord.h
 * Author: tuannguyen
 *
 * Created on 11 August 2015, 5:45 PM
 */



#ifndef FILERECORD_H
#define	FILERECORD_H

#include <iostream>
#include <time.h>
#include <cmath>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include "Database.h"
#include "cppconn/prepared_statement.h"
using namespace std;


class FileRecord {
public:
    const static int BLOCK_SIZE;
    FileRecord(string filename);
    ~FileRecord(); 
    string getFileName() const{ return fileName;} 
    string getTempName() const {  return tempName;} 
    struct timespec getModifyTime() const { return modifyTime; }  
    long getFileLength() const { return fileLength;}  
    int getNVersions() const{ return nVersions;} 
    int getReferenceNumber()const {return refNumber;} 
    int getCurHash() const{ return curHash;} 
    int getOvHash() const { return ovHash;}
    vector<int>* getVersionIDs();
    vector<int>* getBlockHashes();
    vector<string>* getComments();
    
    void setFileLength(long) throw (invalid_argument);
    void setNVersions(int) throw (invalid_argument);
    void setCurHash(int);
    void setOvHash(int);
    void setBlockAtIndex(int, int) throw (out_of_range);
    void setModifyTime(struct timespec);
    void setRefVersion(int) throw (invalid_argument);
    void setTempName(string);
    void saveFileRecToDatabase() throw (SQLException);
    bool getDataFromDatabase() throw (SQLException);
    void createData() throw (runtime_error);
    
private:
    Database database;
    string fileName;
    string tempName;
    struct timespec modifyTime;
    long fileLength;
    int nVersions;
    int curHash;
    int refNumber;
    int ovHash;
    vector<string> comments;
    vector<int> blockHashes;
    vector<int> versionIds;
    void fileHashing();
};

#endif	/* FILERECORD_H */

