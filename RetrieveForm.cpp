/*
 * File:   RetrieveForm.cpp
 * Author: bx830
 *
 * Created on 28 August 2015, 6:12 PM
 */

#include "RetrieveForm.h"
#include "MyWindow.h"
#include "MyTableModel.h"

RetrieveForm::RetrieveForm() {
    widget.setupUi(this);
    connect(widget.selectButton, SIGNAL(clicked()), this, SLOT(SelectFile()));
    connect(widget.ok_button, SIGNAL(clicked()), this, SLOT(ok()));
    connect(widget.cancel_button, SIGNAL(clicked()), this, SLOT(cancel()));
}

RetrieveForm::~RetrieveForm() {
}

void RetrieveForm::SelectFile() {
    QString fileName = QFileDialog::getExistingDirectory(this, "Select directory", ".", QFileDialog::ShowDirsOnly);
    if (!fileName.isEmpty()) {

        widget.directory->setText(fileName);
    }
}

void RetrieveForm::cancel() {

    setResult(CANCEL);
    accept();
}

void RetrieveForm::ok() {
    string directory = widget.directory->text().toStdString();
    if (directory.empty()) {
        QMessageBox message;
        message.setWindowTitle("Error");
        message.setText("Please choose the directory");
        message.setStandardButtons(QMessageBox::Yes);
        message.setDefaultButton(QMessageBox::Yes);
        message.exec();
    }
    string file = widget.filename->text().toStdString();
    if (file.empty()) {
        QMessageBox message;
        message.setWindowTitle("Error");
        message.setText("Please enter the file name");
        message.setStandardButtons(QMessageBox::Yes);
        message.setDefaultButton(QMessageBox::Yes);
        message.exec();
    }
    fileName = directory + "/" + file;
    setResult(OK);
    accept();
}

string RetrieveForm::getFileName() {
    return fileName;
}