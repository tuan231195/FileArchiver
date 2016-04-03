/* 
 * File:   MyTableModel.h
 * Author: mz430
 *
 * Created on 19 August 2015, 4:46 PM
 */

#ifndef MYTABLEMODEL_H
#define	MYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QtGui>
#include <vector>
#include "VersionInfo.h"
using namespace std;
class MyRecord;
typedef VersionInfo RecordPtr;

class MyTableModel : public QAbstractTableModel{
    Q_OBJECT
public:
    MyTableModel(QObject *parent);
    void addTheData(vector<RecordPtr> data){
        beginResetModel();
        recordsCollection = data;
        endResetModel();
        
    }
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void addRecord(RecordPtr newone);
private:
    //Don't assume ownership, don't delete on destruct
    vector<RecordPtr> recordsCollection;
    //Disallow value operations
    MyTableModel& operator = (const MyTableModel&);
    MyTableModel(const MyTableModel& orig);
};

#endif	/* MYTABLEMODEL_H */

