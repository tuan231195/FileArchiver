/*
 * File:   MyWindow.cpp
 * Author: mz430
 *
 * Created on 19 August 2015, 3:24 PM
 */

#include "MyWindow.h"
#include "MyTableModel.h"
#include "MyRecord.h"
#include "getCommentForm.h"
#include "RetrieveForm.h"
using namespace std;

MyWindow::MyWindow() {
    widget.setupUi(this);

    tablemodel = new MyTableModel(0);
    widget.tableView->setModel(tablemodel);
    widget.tableView->resizeRowsToContents();
    widget.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.tableView->setColumnWidth(0, 100);
    widget.tableView->setColumnWidth(1, 250);
    widget.tableView->setColumnWidth(2, 150);

    //We have to put in the event handling links
    connect(widget.FileSelector, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(widget.SaveCurrent, SIGNAL(clicked()), this, SLOT(saveCurrent()));
    connect(widget.RetrieveVersion, SIGNAL(clicked()), this, SLOT(retrieveVersion()));
    connect(widget.SetReference, SIGNAL(clicked()), this, SLOT(setReference()));
    connect(widget.ShowComment, SIGNAL(clicked()), this, SLOT(showComment()));
connect(
            widget.tableView,
            SIGNAL(clicked()), this,
            SLOT(itemSelection(const QModelIndex&))
            );

    connect(
            widget.tableView,
            SIGNAL(clicked(const QModelIndex&)), this,
            SLOT(itemSelection(const QModelIndex&))
            );

    enableButton(false);
}

MyWindow::~MyWindow() {
}

void MyWindow::retrieveVersionDataForFile(string fileName) {
    try {
        infos = fileArchiver.getVersionInfos(fileName);
        tablemodel->addTheData(infos);
        //widget.tableView->resizeColumnsToContents();
        widget.tableView->setAutoFillBackground(0);
    } catch (SQLException & e) {
        showMessage("Error", e.what());
    }


}

void MyWindow::chooseFile() {

    QString fileName = QFileDialog::getOpenFileName(this, "Select file", ".", "All Files (*)");
    if (!fileName.isEmpty()) {
        // A file was picked, set the pictureField to match
        //        getCommentForm commentForm;
        //        commentForm.exec();
        widget.fileField->setText(fileName);
        retrieveVersionDataForFile(fileName.toStdString());
        this->fileName = fileName.toStdString();
    }
    enableButton(false);
}

void MyWindow::saveCurrent() {
    enableButton(false);
    getCommentForm form;

    int result = form.exec();
    try {
        if (result == getCommentForm::OK) {
            string comment = form.getComment();
            if (!fileArchiver.exists(fileName)) {
                fileArchiver.insertNew(fileName, comment);
                showMessage("Save current version", "New version inserted");
            } else if (!fileArchiver.differ(fileName))//if save a same file
            {
                showMessage("Error", "The file has NOT been modified,no save required");
                return;
            } else {
                fileArchiver.update(fileName, comment);
                showMessage("Save current version", "Modified and updated");

            }
            retrieveVersionDataForFile(fileName);
        }
    } catch (SQLException &e) {
        showMessage("Error", e.what());
    }

    return;
}

void MyWindow::retrieveVersion() {
    RetrieveForm retrieveForm;
    int result = retrieveForm.exec();
    if (result == RetrieveForm::OK) {
        try {
            fileArchiver.retrieveVersion(selectedRow, fileName, retrieveForm.getFileName());
            showMessage("Success", "File retrieved");
            enableButton(false);
        } catch (SQLException &e) {
            showMessage("Error", e.what());
        } catch (runtime_error &e) {
            showMessage("Error", "File " + retrieveForm.getFileName() + " already exists");
        }
    }
    
}

void MyWindow::setReference() {
    QMessageBox message;
    message.setWindowTitle("Set reference version");
    message.setText("Set reference version");
    message.setIcon(QMessageBox::Question);
    QAbstractButton *myYesButton = message.addButton(trUtf8("OK"), QMessageBox::YesRole);
    message.exec();
    if (message.clickedButton() == myYesButton)//conduct setting reference version operation
    {
        getCommentForm form;
        int result = form.exec();
        if (result == getCommentForm::OK) {
            try {
                fileArchiver.setReference(fileName, selectedRow, form.getComment());
                retrieveVersionDataForFile(fileName);
            } catch (SQLException &e) {
                showMessage("Error", e.what());
            }
              
        }
    } else //cancel the operation
    {
        return;
    }

    enableButton(false);
}

void MyWindow::showComment() {
    
    //this function has to get the comment from database.
    try {
        showMessage("Comment for selected version!", fileArchiver.getComment(fileName, selectedRow));
    } catch (SQLException &e) {
        showMessage("Error", e.what());
    }
    enableButton(false);
}

void MyWindow::itemSelection(const QModelIndex& index) {
    selectedRow = index.row();
    enableButton(true);
    if (selectedRow == 0)
    {
        widget.SetReference->setEnabled(false);
    }
}

void MyWindow::enableButton(bool visible) {
    widget.RetrieveVersion->setEnabled(visible);
    widget.SetReference->setEnabled(visible);
    widget.ShowComment->setEnabled(visible);
}

void MyWindow::showMessage(string title, string text) {
    QMessageBox message;
    message.setWindowTitle(title.c_str());
    message.setText(text.c_str());
    message.setIcon(QMessageBox::Information);
    message.setStandardButtons(QMessageBox::Yes);
    message.setDefaultButton(QMessageBox::Yes);
    message.exec();
}