/* 
 * File:   MyWindow.h
 * Author: mz430
 *
 * Created on 19 August 2015, 3:24 PM
 */

#ifndef _MYWINDOW_H
#define	_MYWINDOW_H

#include "ui_MyWindow.h"
#include "FileArchiver.h"
using namespace std;

class MyRecord;

class MyTableModel;

class MyWindow : public QMainWindow {
    Q_OBJECT
public:
    MyWindow();
    virtual ~MyWindow();
    
public slots:
    void chooseFile();
    void saveCurrent();
    void retrieveVersion();
    void setReference();
    void retrieveVersionDataForFile(string fileName);
    void showComment();
    void itemSelection(const QModelIndex & index);
    void enableButton(bool visible);
    void showMessage(string title, string text);
private:
    int selectedRow;
    FileArchiver fileArchiver;
    vector<VersionInfo> infos;
    Ui::MyWindow widget;
    MyTableModel *tablemodel;
    string fileName;
};

#endif	/* _MYWINDOW_H */
