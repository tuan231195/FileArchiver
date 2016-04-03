/* 
 * File:   FileArchiver.cpp
 * Author: tuannguyen
 * 
 * Created on 29 August 2015, 10:54 PM
 */

#include "FileArchiver.h"
#include "Utils.h"
#include <qt4/Qt/qbytearray.h>
#include <cstdlib>
#include <stdio.h>


FileArchiver::FileArchiver() {
    localFileRec = 0;
    lastUsedVersion = 0;
}

FileArchiver::~FileArchiver()
{
    releaseResource();
}

void FileArchiver::unzipRetrievedFile(string filePath)
{
    string command = "gunzip " + filePath; 
    system(command.c_str());
}


bool FileArchiver::exists(string fileName) throw (SQLException)
{
    bool exists = false;
    Connection * connection = database.getConnection();
    PreparedStatement *stmt = connection-> prepareStatement("SELECT COUNT(*) FROM filerec WHERE filename = ?");
    stmt -> setString(1, fileName);
    ResultSet *rs = stmt -> executeQuery();
    if (rs -> next()) 
    {
        int count = rs -> getInt(1);
        if (count != 0)
        {
            exists = true;
        }
    }
    delete rs;
    delete stmt;
    return exists;
}

bool FileArchiver::differ(string fileName) throw (SQLException, runtime_error)
{   
    int oldHash, newHash;
    releaseResource();
    //create a new File record
    localFileRec = new FileRecord(fileName);
    localFileRec->createData();
    newHash = localFileRec->getCurHash();
    
    //get old hash value from the database
    Connection * connection = database.getConnection();
    PreparedStatement *stmt = connection-> prepareStatement("SELECT curhash FROM filerec WHERE filename = ?");
    stmt -> setString(1, fileName);
    ResultSet *rs = stmt -> executeQuery();
    if (rs -> next()) 
    {
        oldHash = rs -> getInt(1);
    }
    delete stmt;
    delete rs; 
    return (oldHash != newHash);
}

void FileArchiver::releaseResource()
{
    if (localFileRec != NULL)
    {
        delete localFileRec;
        localFileRec = NULL;
    }
        
    if (lastUsedVersion != NULL)
    {
        delete lastUsedVersion;
        lastUsedVersion = NULL;
    }
}

void FileArchiver::insertNew(string fileName, string comment) throw (SQLException, runtime_error)
{
    releaseResource();
    localFileRec = new FileRecord(fileName);
    localFileRec->createData();
    Connection *connection = database.getConnection();
    char * tempName = createZipFile(fileName);
    ifstream is(tempName, ios::binary);
    PreparedStatement *pstmt = connection->prepareStatement("INSERT INTO blobtable VALUES (?, ?)");
    pstmt -> setString(1, tempName);
    pstmt->setBlob(2, &is);
    pstmt ->executeUpdate();
    delete pstmt;
    
    localFileRec->setTempName(tempName);
    localFileRec->saveFileRecToDatabase();

    saveCommentToDatabase(fileName, comment, 0);
    remove(tempName);        
    delete tempName;
    
}

void FileArchiver::saveCommentToDatabase(string fileName, string comment, int commentNum) throw (SQLException)
{
    Connection *connection = database.getConnection();
    PreparedStatement *pstmt = connection -> prepareStatement("INSERT INTO commentstable(fileref, commentnum, commenttxt) VALUES (?,?,?)");
    pstmt -> setString(1, fileName);
    pstmt -> setInt(2, commentNum);
    pstmt -> setString(3, comment);
    pstmt -> executeUpdate();
    delete pstmt;
}


Block* FileArchiver::readBlock(string fileName, int blkIdx) 
{
    
    ifstream is(fileName.c_str());
    is.seekg(0, ios::end);
    long size = is.tellg();
    int num_blocks = ceil(size * 1.0/FileRecord::BLOCK_SIZE);
    if (blkIdx < 0 || blkIdx >= num_blocks)
    {
        throw out_of_range("Out of range exception from readBlock: " + to_string(blkIdx));
    }
    
    is.seekg(blkIdx * FileRecord::BLOCK_SIZE, ios::beg);
    Block* fileBlock = new Block();
    char block[FileRecord::BLOCK_SIZE];
    is.read(block, FileRecord::BLOCK_SIZE);
    int numRead = is.gcount();
    fileBlock->blocknum = blkIdx;
    fileBlock -> hash = blockHashing(block, numRead , NULL);
    fileBlock -> length = compressBlock(block, numRead, fileBlock -> data);  
    is.close();
    return fileBlock;
}

void FileArchiver::getVersion(string fileName, int versionIdx)
{
    if (lastUsedVersion != NULL)
    {
        delete lastUsedVersion;
    }
    lastUsedVersion = new FileRecord(fileName);
    lastUsedVersion->getDataFromDatabase();
    vector<int> *versionIds = lastUsedVersion->getVersionIDs();
    
    //the current version is the reference version
    if (versionIds->size() == 0){
        return;
    }
    if (versionIdx == -1)
    {
        versionIdx = versionIds->size() - 1;
    }
    vector<int> *blkHashes = lastUsedVersion->getBlockHashes();
    for (int i = 0; i <= versionIdx;i ++)
    {
        int id = (*versionIds)[i];
        VersionRec versionRec(id, fileName);
        versionRec.getDataFromDatabase();
        long length =  versionRec.getLength();
        size_t numBlocks = ceil(length * 1.0/FileRecord::BLOCK_SIZE);
        //the current file is smaller than the file record saved in the database
        if (numBlocks < blkHashes-> size())
        {
            blkHashes-> erase(blkHashes->begin() + numBlocks, blkHashes->end());
        }
        vector<Block*> *blocks = versionRec.getBlocks();
        for (size_t i =0 ; i < blocks->size(); i++)
        {
            size_t blknum = (*blocks)[i]->blocknum;
            int hash = (*blocks)[i]->hash;
            if (blknum < numBlocks)
            {
                (*blkHashes)[blknum] = hash;
            }
            else
            {
                blkHashes -> push_back(hash);
            }
        }
    }
}

void FileArchiver::getDetailsOfLastSaved(string fileName) throw (SQLException)
{
    getVersion(fileName, -1);
}

void FileArchiver::update(string fileName, string comment) throw (SQLException)
{
    getDetailsOfLastSaved(fileName);
    vector<int> * curHashes = localFileRec->getBlockHashes();
    vector<int> * oldHashes = lastUsedVersion->getBlockHashes();
    VersionRec newVersion(fileName);
    newVersion.setHash(localFileRec->getCurHash());
    newVersion.setLength(localFileRec ->getFileLength());
    newVersion.setVersionNum(lastUsedVersion->getReferenceNumber() + lastUsedVersion->getNVersions());
    newVersion.setModifyTime(localFileRec->getModifyTime());
    for (size_t i = 0; i < curHashes->size(); i++)
    {
        if (i < oldHashes->size() && (*oldHashes)[i] == (*curHashes)[i])
            continue;
        Block * block = readBlock(fileName, i);
        newVersion.addBlock(block);
    }
    saveCommentToDatabase(fileName, comment, newVersion.getVersionNum());
    newVersion.saveDataToDatabase();
    Connection *connection = database.getConnection();
    PreparedStatement *pstmt = connection->prepareStatement("UPDATE filerec SET curhash = ?, nversions = ? WHERE filename = ?");
    pstmt -> setInt(1, newVersion.getHash());
    pstmt -> setInt(2, lastUsedVersion->getNVersions() + 1);
    pstmt -> setString(3, fileName);
    pstmt -> executeUpdate();
    delete pstmt;
    
    lastUsedVersion->setNVersions(lastUsedVersion -> getNVersions() + 1);
    lastUsedVersion->setCurHash(newVersion.getHash());
}

//    void getVersionInfo(string);
void FileArchiver::retrieveVersion(int versionIdx, string fileName, string savedFile) throw  (SQLException, runtime_error, out_of_range)
{
    if (!exists(fileName))
    {
        throw runtime_error("File does not exists in the database");
        return;
    }
    if (access(savedFile.c_str(), F_OK) == 0)
    {
        throw runtime_error("File already exists in the local file system. Please choose another file name");
        return;
    }
    
    if (lastUsedVersion != NULL)
    {
        delete lastUsedVersion;
    }
    
    lastUsedVersion = new FileRecord(fileName);
    lastUsedVersion->getDataFromDatabase();
    long length = lastUsedVersion->getFileLength();
    vector<int> * versionIds = lastUsedVersion->getVersionIDs();
    if (versionIdx < 0 || versionIdx > (*versionIds).size())
    {
        throw out_of_range("Invalid argument exception from retrieve Version :" + to_string(versionIdx));
        return;
    }
    
    char block[FileRecord::BLOCK_SIZE];
    vector<int> * blkHashes = lastUsedVersion -> getBlockHashes();
    retrieveRefVersion();
    if (versionIdx != 0)
    {
        fstream fileStream(lastUsedVersion->getTempName().c_str(), ios::in | ios::out);
        fstream destStream(savedFile.c_str(), ios::out);
        for (int i = 0; i < versionIdx; i++)
        {
            int id = (*versionIds)[i];
            VersionRec versionRec(id, fileName);
            versionRec.getDataFromDatabase();
            long versionLength = versionRec.getLength();
            if (i == versionIdx -1)
            {
                struct timespec mTime;
                mTime.tv_nsec = versionRec.getModifyTime().tv_nsec;
                mTime.tv_sec = versionRec.getModifyTime().tv_sec;
                int hash = versionRec.getHash();
                lastUsedVersion->setFileLength(versionLength);
                lastUsedVersion->setModifyTime(mTime);
                lastUsedVersion -> setOvHash(hash);
            }


            size_t numBlocks = ceil(length * 1.0/FileRecord::BLOCK_SIZE);
            
           
            //the current file is smaller than the file record saved in the database
            if (numBlocks < blkHashes-> size())
            {
                blkHashes-> erase(blkHashes->begin() + numBlocks, blkHashes->end());
            }
            
            vector<Block*> *versionBlocks = versionRec.getBlocks();
            for(size_t i = 0; i < versionBlocks->size(); i ++)
            {
                int blknum = (*versionBlocks)[i]->blocknum;
                char * compressedData = (*versionBlocks)[i]->data;
                int dataLength = (*versionBlocks)[i]->length;
                int hash = (*versionBlocks)[i]->hash;
                if (blknum < numBlocks)
                {
                    (*blkHashes)[blknum] = hash;
                }
                else
                {
                    if (blknum == blkHashes->capacity())
                    {
                        blkHashes->reserve(blkHashes->capacity()*2);
                    }
                    blkHashes -> push_back(hash);
                }
                char * data;
                int blockSize = deCompressBlock(compressedData, dataLength,  data);
                fileStream.seekp(blknum * FileRecord::BLOCK_SIZE);
                fileStream.write(data, blockSize);
                delete data;
            }
            length = versionLength;
        }
        
        fileStream.seekg(0, ios::beg);
        //copy the file to specified location
        long remaining = lastUsedVersion->getFileLength();
        int byteWrite = FileRecord::BLOCK_SIZE;
        while (remaining > 0)
        {
            if (remaining < FileRecord::BLOCK_SIZE)
            {
                byteWrite = remaining;
            }
            fileStream.read(block, byteWrite);
            destStream.write(block, byteWrite);  
            remaining -= byteWrite;
        }
        fileStream.close();
        destStream.close();

    }
    else
    {
        string moveStatement = "mv " + lastUsedVersion->getTempName() + " " + savedFile; 
        system(moveStatement.c_str());
    }
    
    remove(lastUsedVersion->getTempName().c_str());
}

 
char * FileArchiver::createZipFile(string filePath)
{
    char * tempName = tempnam("/tmp", NULL); 
    string command = "gzip -c " + filePath + " > " + tempName; 
    system(command.c_str());
    return tempName;
}


int FileArchiver::deCompressBlock(char * compressed, int size, char*& ret)
{
    QByteArray data(compressed, size);
    QByteArray decompressed = qUncompress(data);
    ret = new char[decompressed.size()];
    
    memcpy(ret, decompressed.data(), decompressed.size());
    
    return decompressed.size();
}

int FileArchiver::compressBlock(char * block,int size, char *&ret)
{
    QByteArray data = QByteArray(block, size);
    QByteArray compressed = qCompress(data, 9);
    ret = new char[compressed.size()];
    memcpy(ret, compressed.data(), compressed.size());
    return compressed.size();
}


void FileArchiver::retrieveRefVersion() throw (SQLException)
{
    istream *is;
    char block[FileRecord::BLOCK_SIZE];
    string tempFilePath = lastUsedVersion->getTempName() + ".gz";
    ofstream os(tempFilePath.c_str(), ios::binary);
    Connection * connection = database.getConnection();
    PreparedStatement * pstmt = connection->prepareStatement("SELECT filedata FROM blobtable WHERE tempname = ?");
    pstmt -> setString(1, lastUsedVersion->getTempName());
    ResultSet * rs = pstmt -> executeQuery();
    if (rs -> next())
    {
        is = rs -> getBlob(1);
    }
    while (is->read(block, FileRecord::BLOCK_SIZE))
    {
        os.write(block, FileRecord::BLOCK_SIZE);
    }
    if (is->gcount() != 0)
    {
        os.write(block, is->gcount());
    }
    os.close();
    unzipRetrievedFile(tempFilePath);
}


void FileArchiver::setReference(string fileName, int refIdx, string comment) throw (SQLException, runtime_error, out_of_range)
{
    char * tempName = tempnam("/tmp", NULL); 
    retrieveVersion(refIdx, fileName, tempName);
    
    char* compressedFileName = createZipFile(tempName);
    Connection *connection = database.getConnection();
    
    
    ifstream is(compressedFileName, ios::binary);
    PreparedStatement * pstmt = connection->prepareStatement("INSERT INTO blobtable VALUES (?, ?)");
    pstmt -> setString(1, compressedFileName);
    pstmt->setBlob(2, &is);
    pstmt ->executeUpdate();
    delete pstmt;
    
    string oldTmpName  = lastUsedVersion->getTempName();
    int newRefNum = lastUsedVersion -> getReferenceNumber() + refIdx;
    
    
    //save file Record
    lastUsedVersion -> setRefVersion(newRefNum);
    lastUsedVersion -> setTempName(compressedFileName);
    lastUsedVersion -> setNVersions(lastUsedVersion -> getNVersions() - refIdx);
    lastUsedVersion -> saveFileRecToDatabase();
    vector<int> * ids = lastUsedVersion -> getVersionIDs();
    
    //delete old blob
    pstmt = connection->prepareStatement("DELETE FROM blobtable WHERE tempname = ?");
    pstmt -> setString(1, oldTmpName);
    pstmt -> executeUpdate();
    delete pstmt;
    
    //update comment
    pstmt = connection -> prepareStatement("UPDATE commentstable SET commenttxt = ? WHERE fileref = ? AND commentnum = " + to_string(newRefNum));
    pstmt -> setString(1, comment);
    pstmt -> setString(2, fileName);
    pstmt -> executeUpdate(); 
    
    //delete old comments
    pstmt = connection -> prepareStatement("DELETE FROM commentstable WHERE fileref = ? ORDER BY commentnum LIMIT " + to_string(refIdx));
    pstmt -> setString(1, fileName);
    pstmt -> executeUpdate(); 
    delete pstmt;
    
    //delete old version records and blocks associated with each old version record
    Statement *stmt = connection -> createStatement();
    for (int i = 0; i < refIdx; i++)
    {
        int id = (*ids)[i];
        stmt -> executeUpdate("DELETE FROM blktable WHERE version = " + to_string(id));
        stmt -> executeUpdate("DELETE FROM versionrec WHERE idversionrec = " + to_string(id));
    }
    delete stmt;
    delete tempName;
    delete compressedFileName;
    remove(compressedFileName);
    remove(tempName);
}

string FileArchiver::getComment(string fileName, int commentIdx) throw (SQLException) 
{
    Connection * connection = database.getConnection();
    PreparedStatement* pstmt  = connection -> prepareStatement("SELECT commenttxt FROM commentstable WHERE fileref = ? ORDER BY commentnum LIMIT " + to_string(commentIdx) + ", 1 ");
    pstmt -> setString(1, fileName);
    string s;
    ResultSet * rs = pstmt -> executeQuery();
    if (rs -> next())
    {
        s = rs -> getString(1);
    }
    delete pstmt;
    delete rs;
    return s;
}

vector<VersionInfo> FileArchiver::getVersionInfos(string fileName) throw (SQLException)
{
    infos.clear();
    Connection * connection = database.getConnection();
    PreparedStatement* pstmt  = connection -> prepareStatement("SELECT length,mtsec, mtnsec, currentversion  FROM filerec WHERE filename = ?");
    pstmt -> setString(1, fileName);
    ResultSet *rs = pstmt -> executeQuery();
    if (rs -> next())
    {
        VersionInfo info;
        info.length = rs -> getInt64(1);
        info.mTime.tv_sec = rs -> getInt64(2);
        info.mTime.tv_nsec = rs -> getInt64(3);
        info.versionNum = rs -> getInt(4);
        infos.push_back(info);
    }
    delete pstmt;
    delete rs;
    
    pstmt  = connection -> prepareStatement("SELECT length,mtsec, mtnsec, versionnum  FROM versionrec WHERE fileref = ? ORDER BY versionnum");
    pstmt -> setString(1, fileName);
    rs = pstmt -> executeQuery();
    while (rs -> next())
    {
        VersionInfo info;
        info.length = rs -> getInt64(1);
        info.mTime.tv_sec = rs -> getInt64(2);
        info.mTime.tv_nsec = rs -> getInt64(3);
        info.versionNum = rs -> getInt(4);
        infos.push_back(info);
    }
    delete pstmt;
    delete rs;
    return infos;
}
