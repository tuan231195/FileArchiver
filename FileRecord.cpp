/* 
 * File:   FileRecord.cpp
 * Author: tuannguyen
 * 
 * Created on 11 August 2015, 5:45 PM
 */


#include "FileRecord.h"
#include "Utils.h"

const int FileRecord::BLOCK_SIZE = 4096;
FileRecord::FileRecord(string name) {
    fileName = name;
    tempName = "";
    nVersions = -1;
    refNumber = -1;
    ovHash = -1;
    curHash = -1;
    modifyTime.tv_nsec = -1;
    modifyTime.tv_sec = -1;
}

FileRecord::~FileRecord() {
}

void FileRecord::setNVersions(int nVersions) throw (invalid_argument)
{
    if (nVersions < 0)
    {
        string msg = "Invalid Argument exception from FileRecord::setNVersions. nVersions is " + to_string(nVersions);
        throw invalid_argument(msg);
    }
    this-> nVersions = nVersions;
}

void FileRecord::setFileLength(long fileLength) throw (invalid_argument)
{
    if (fileLength < 0)
    {
        string msg = "Invalid Argument exception from FileRecord::setFileLength. length is " + to_string(fileLength);
        throw invalid_argument(msg);
    }
    this->fileLength = fileLength;
}

void FileRecord::setCurHash(int fileHash)
{
    this -> curHash = fileHash;
}

void FileRecord::setOvHash(int fileHash)
{
    this -> ovHash = fileHash;
}


void FileRecord::setModifyTime(struct timespec mTime)
{
    modifyTime = mTime;
}

void FileRecord::setTempName(string tmp)
{
    tempName = tmp;
}

void FileRecord::setRefVersion(int refVer) throw (invalid_argument)
{
    if (refVer < 0)
    {
        string msg = "Invalid Argument exception from FileRecord::setRefVersion. refVer is " + to_string(refVer);
        throw invalid_argument(msg);
    }
    refNumber = refVer;
}


void FileRecord::saveFileRecToDatabase() throw (SQLException)
{
    int count;
    const char* sqlQuery = "select count(*) from filerec where filename = ?";
    Connection * connection  = database.getConnection();
    PreparedStatement *pstmt = NULL;
    ResultSet * rs = NULL;
    pstmt = connection -> prepareStatement(sqlQuery);
    pstmt -> setString(1, fileName);
    rs = pstmt -> executeQuery();
    if (rs -> next())
    {
        count = rs -> getInt(1);
    }
    delete pstmt;
    if (count == 0)
    {
        const char * insertQuery = "INSERT INTO filerec VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)";
        pstmt = connection -> prepareStatement(insertQuery);
        pstmt -> setString(1, fileName);
        pstmt -> setInt(2, curHash);
        pstmt -> setInt(3, ovHash);
        pstmt -> setInt(4,refNumber);
        pstmt -> setInt(5, nVersions);
        pstmt -> setInt64(6, fileLength);
        pstmt -> setInt64(7, modifyTime.tv_nsec);
        pstmt -> setInt64(8, modifyTime.tv_sec);
        pstmt -> setString(9, tempName);
        pstmt->executeUpdate();
        
        for (int i = 0; i < blockHashes.size(); i++)
        {
            pstmt = connection->prepareStatement("INSERT INTO fileblkhashes(fileref, blknum, hashval)  VALUES (?, ?, ?)");
            pstmt -> setString(1, fileName);
            pstmt -> setInt(2, i);
            pstmt -> setInt(3, blockHashes[i]);
            pstmt -> executeUpdate();
            delete pstmt;
        }
    }
    else
    {
        const char * updateQuery = "UPDATE filerec SET curhash = ?, ovhash = ?, currentversion = ? , nversions = ? , length = ?, mtnsec = ?, mtsec = ? , tempname =? WHERE filename = ?";
        pstmt = connection -> prepareStatement(updateQuery);
        pstmt -> setInt(1, curHash);
        pstmt -> setInt(2, ovHash);
        pstmt -> setInt(3,refNumber);
        pstmt -> setInt(4, nVersions);
        pstmt -> setInt64(5, fileLength);
        pstmt -> setInt64(6, modifyTime.tv_nsec);
        pstmt -> setInt64(7, modifyTime.tv_sec);
        pstmt -> setString(8, tempName);
        pstmt -> setString(9, fileName);
        
        pstmt->executeUpdate();
        delete pstmt;
        
        const char * deleteStatement = "DELETE FROM fileblkhashes WHERE fileref = ?";
        pstmt = connection -> prepareStatement(deleteStatement);
        pstmt -> setString(1, fileName);
        pstmt -> executeUpdate();
        delete pstmt;
        
        for (int i = 0; i < blockHashes.size(); i++)
        {
            pstmt = connection->prepareStatement("INSERT INTO fileblkhashes(fileref, blknum, hashval)  VALUES (?, ?, ?)");
            pstmt -> setString(1, fileName);
            pstmt -> setInt(2, i);
            pstmt -> setInt(3, blockHashes[i]);
            pstmt -> executeUpdate();
            delete pstmt;
        }
    }

    delete rs;
}

bool FileRecord::getDataFromDatabase() throw (SQLException)
{
    const char* fileRecSqlQuery = "select curhash, ovhash, currentversion, nversions, length, mtnsec, mtsec, tempname from filerec where filename = ?";
    Connection *connection = database.getConnection();
    PreparedStatement *pstmt = NULL;
    ResultSet *rs = NULL;
    pstmt = connection ->prepareStatement(fileRecSqlQuery);
    pstmt -> setString(1, fileName);
    rs = pstmt -> executeQuery();
    if (!(rs -> next()))
    {
        delete pstmt;
        delete rs;
        return false;
    }
   
    curHash = rs-> getInt(1);
    ovHash = rs -> getInt(2);
    refNumber = rs -> getInt(3);
    nVersions = rs -> getInt(4);
    fileLength = rs -> getInt64(5);
    modifyTime.tv_sec = rs -> getInt64(6);
    modifyTime.tv_nsec = rs -> getInt64(7);
    tempName = rs -> getString(8);
    delete pstmt;
    delete rs;
    return true;
}

vector<int>* FileRecord::getVersionIDs()
{
    if (versionIds.size() == 0)
    {
        Connection *connection = database.getConnection();
        PreparedStatement *pstmt = NULL;
        ResultSet *rs = NULL;
        const char *verIdSqlQuery = "Select idversionrec FROM versionrec WHERE fileref = ? ORDER BY versionnum";
        pstmt = connection->prepareStatement(verIdSqlQuery);
        pstmt -> setString(1, fileName);
        rs = pstmt -> executeQuery();
        if (rs -> next())
        {
            do
            {
                int s = rs -> getInt(1);
                versionIds.push_back(s);
            }
            while (rs -> next());
        }
        delete rs;
        delete pstmt;
    }
    
    return &versionIds;
}

vector<string>* FileRecord::getComments()
{
    if (comments.size() == 0)
    {
        Connection *connection = database.getConnection();
        PreparedStatement *pstmt = NULL;
        ResultSet *rs = NULL;
        const char * commentSqlQuery = "SELECT  commenttxt FROM commentstable WHERE fileref = ? ORDER BY commentnum";
        pstmt = connection->prepareStatement(commentSqlQuery);
        pstmt -> setString(1, fileName);
        rs = pstmt -> executeQuery();
        if (rs -> next())
        {
            do
            {
                string s = rs -> getString(1);
                comments.push_back(s);
            }
            while (rs -> next());
        }
        delete rs;
        delete pstmt;
    }
    return &comments;
    
}

vector<int>* FileRecord::getBlockHashes() {
    if (blockHashes.size() == 0)
    {
        Connection *connection = database.getConnection();
        PreparedStatement *pstmt = NULL;
        ResultSet *rs = NULL;
        const char * blockSqlQuery = "SELECT  hashval FROM fileblkhashes WHERE fileref = ? ORDER BY blknum";
        pstmt = connection->prepareStatement(blockSqlQuery);
        pstmt -> setString(1, fileName);
        rs = pstmt -> executeQuery();
        if (rs -> next())
        {
            do
            {
                int hash = rs -> getInt(1);
                blockHashes.push_back(hash);
            }
            while (rs -> next());
        }
        delete rs;
        delete pstmt;
    }
    return &blockHashes;
}

void FileRecord::createData() throw (runtime_error)
{
    struct stat info;
    ifstream is;
    char block[BLOCK_SIZE];
    static const int c1 = 0xcc9e2d51;
    static const int c2 = 0x1b873593;
    static const int r1 = 15;
    
    int result = stat(fileName.c_str(), &info);
    if (result != 0)
    {
        throw runtime_error("File does not exist.");
    }
    if ((info.st_mode & S_IROTH) != S_IROTH)
    {
        throw runtime_error("Read permission is required");
    }
    nVersions = 1;
    refNumber = 0;
    modifyTime = info.st_mtim;
    fileLength = info.st_size;
    if (fileLength == 0)
    {
        throw runtime_error("File is empty");
    }
    
    curHash = 1;
    
    int numBlocks = ceil(fileLength * 1.0/BLOCK_SIZE);
    blockHashes.reserve(numBlocks + 2);
    
    is.open(fileName);
    while (is.read(block, BLOCK_SIZE))
    {
        int hash = blockHashing(block, BLOCK_SIZE, &curHash);
        blockHashes.push_back(hash);
    }
    if (is.gcount() != 0)
    {
        int hash = blockHashing(block, is.gcount(), &curHash);
        blockHashes.push_back(hash);
    }
    
    //finish the file hashing
    int k1 = 0;
    char lastChars[3];
    is.clear();
    
    int num = fileLength & 3;
    is.seekg(-num, ios::end);
    is.read(lastChars, num);
    switch (num) {
	case 3:
		k1 ^= lastChars[2] << 16;
	case 2:
		k1 ^= lastChars[1] << 8;
	case 1:
		k1 ^= lastChars[0];
		k1 *= c1;
		k1 = (k1 << r1) | (k1 >> (32 - r1));
		k1 *= c2;
		curHash ^= k1;
	}

    curHash ^= fileLength;
    curHash ^= (curHash >> 16);	
    curHash *= 0x85ebca6b;
    curHash ^= (curHash >> 13);
    curHash *= 0xc2b2ae35;
    curHash ^= (curHash >> 16);
    ovHash = curHash;
    is.close();
}
