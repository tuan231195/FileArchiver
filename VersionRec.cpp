/* 
 * File:   VersionRec.cpp
 * Author: tuannguyen
 * 
 * Created on 13 August 2015, 6:45 PM
 */

#include "VersionRec.h"
#include <string.h>


Block::Block()
{
    data = NULL;
    blocknum = -1;
    hash = -1;
    length = -1;
}

Block::~Block() {
    if (data != NULL)
    {
        delete [] data;
    }
}


VersionRec::VersionRec(string fileRef) {
    id = -1;
    this->fileRef = fileRef;
    init();
}

VersionRec::VersionRec(int id, string fileRef)
{
    this->id = id;
    this -> fileRef = fileRef;
    init();
}

VersionRec::~VersionRec()
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i] != NULL)
        {
            delete blocks[i];
        }
    }
}
void VersionRec::init()
{
    length = -1;
    modifyTime.tv_nsec = -1;
    modifyTime.tv_sec = -1;
    versionNum = -1;
    hash = -1;
}

void VersionRec::addBlock(Block* block)
{
    blocks.push_back(block);
}

void VersionRec::setHash(int hash)
{
    this->hash = hash;
}

void VersionRec::setLength(long length) throw (runtime_error)
{
    if (length < 0)
    {
        throw runtime_error("The length of a version record must be greater than 0");
    }
    this-> length = length;
}

void VersionRec::setModifyTime(timespec time)
{
    modifyTime.tv_nsec = time.tv_nsec;
    modifyTime.tv_sec = time.tv_sec;
}

void VersionRec::setVersionNum(int versionNum)
{
    this->versionNum = versionNum;
}

void VersionRec::saveDataToDatabase() throw (SQLException)
{
    const char * versionSQLQuery = "INSERT INTO versionrec(fileref, versionnum, length, mtsec, mtnsec, ovhash) VALUES (?, ?, ?, ?, ?, ?)";
    Connection * connection = database.getConnection();
    PreparedStatement *pstmt = connection ->prepareStatement(versionSQLQuery);
    pstmt -> setString(1, fileRef);
    pstmt->setInt(2, versionNum);
    pstmt -> setInt64(3, length);
    pstmt -> setInt64(4, modifyTime.tv_sec);
    pstmt -> setInt64(5, modifyTime.tv_nsec);
    pstmt -> setInt(6, hash);
    pstmt ->executeUpdate();
    delete pstmt;
    
    //get the last inserted id;
    Statement* stmt = connection->createStatement(); 
    ResultSet* res = stmt->executeQuery("SELECT @@identity AS id"); 
    res->next(); 
    id = res->getInt("id");
     
    delete stmt;
    delete res;
     
    for (int i = 0; i < blocks.size(); i++)
    {
        const char * blockSQLQuery = "INSERT INTO blktable(version, length, blknum, hash, data) VALUES (?, ?, ?, ?, ?)";
        Block * block = blocks[i];
        pstmt = connection ->prepareStatement(blockSQLQuery);
        pstmt -> setInt(1, id);
        pstmt->setInt(2, block->length);
        pstmt -> setInt(3, block->blocknum);
        pstmt -> setInt(4, block->hash);
        pstmt -> setString(5, string(block->data, block->length));
        pstmt ->executeUpdate();
        delete pstmt;
    }
    
}

bool VersionRec::getDataFromDatabase() throw (SQLException)
{
    string versionSQLQuery = "Select versionnum, length, mtsec, mtnsec, ovhash FROM versionrec WHERE idversionrec = " + to_string(id);
    Connection * connection = database.getConnection();
    Statement *stmt = connection->createStatement();
    ResultSet * rs = stmt -> executeQuery(versionSQLQuery.c_str());
    
    if (!(rs -> next()))
    {
        return false;
    }
    
    do
    {
        versionNum = rs ->getInt(1);
        length = rs -> getInt64(2);
        modifyTime.tv_sec = rs -> getInt64(3);
        modifyTime.tv_nsec = rs -> getInt64(4);
        hash = rs -> getInt(5);
    }
    while (rs -> next());
    
    delete stmt;
    delete rs;
    
    string blockSQLQuery = "Select length, blknum, hash, data FROM blktable WHERE version = " + to_string(id);
    stmt = connection -> createStatement();
    rs = stmt ->executeQuery(blockSQLQuery.c_str());
    while (rs -> next())
    {
        Block *b = new Block();
        b -> length = rs -> getInt64(1);
        b ->blocknum = rs ->getInt(2);
        b -> hash = rs -> getInt(3);
        b -> data = new char[b->length];
        memcpy(b->data, rs -> getString(4).c_str(), b->length);
        blocks.push_back(b);
    }
    delete stmt;
    delete rs;
    return true;
}