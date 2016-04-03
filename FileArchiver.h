/* 
 * File:   FileArchiver.h
 * Author: tuannguyen
 *
 * Created on 29 August 2015, 10:54 PM
 */

#ifndef FILEARCHIVER_H
#define	FILEARCHIVER_H

#include <iostream>
#include "Database.h"
#include "VersionInfo.h"
#include "VersionRec.h"
#include "FileRecord.h"
using namespace std;

class FileArchiver {
public:
    FileArchiver();
    ~FileArchiver();
    bool exists(string) throw (SQLException);
    bool differ(string) throw (SQLException, runtime_error);
    void insertNew(string, string) throw (SQLException, runtime_error);
    vector<VersionInfo> getVersionInfos(string) throw (SQLException);
    void update(string, string) throw (SQLException);
    void getDetailsOfLastSaved(string) throw (SQLException);
    void retrieveVersion(int, string, string) throw (SQLException, runtime_error, out_of_range);
    string getComment(string , int ) throw (SQLException);
    void setReference(string, int , string comment) throw (SQLException, runtime_error, out_of_range);
    void releaseResource();
private:
    vector<VersionInfo> infos;
    FileRecord *localFileRec;
    FileRecord *lastUsedVersion;
    Database database;
    void retrieveRefVersion() throw (SQLException);
    char* createZipFile(string filePath);
    void unzipRetrievedFile(string filePath);
    Block* readBlock(string fileName, int);
    void saveCommentToDatabase(string, string, int) throw (SQLException);
    int compressBlock(char *, int, char*&);
    int deCompressBlock(char*, int, char *&);
    FileArchiver(const FileArchiver& orig);
    void getVersion(string, int);
    
};

#endif	/* FILEARCHIVER_H */

