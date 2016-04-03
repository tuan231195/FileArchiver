#include "Database.h"
#include "FileRecord.h"
#include "FileArchiver.h"
#include <iostream>

using namespace std;

int main()
{
    FileArchiver fileArchiver;
    string fileName = "1.jpg";
//    if (fileArchiver.differ(fileName))
//    {
//        fileArchiver.update(fileName, "Third comment");
//    }    
    //fileArchiver.insertNew(fileName, "This is the first comment");
    //fileArchiver.retrieveVersion(1, fileName, "retrieve.jpg");
    vector<VersionInfo> infos = fileArchiver.getVersionInfos(fileName);
    for (VersionInfo info: infos)
    {
        cout << info.length << endl;
        cout << info.versionNum << endl;
        cout << info.mTime.tv_nsec << " " << info.mTime.tv_sec << endl;
    }
    //    
    //fileArchiver.setReference(fileName, 1, "New commit");
    return 0;
}
