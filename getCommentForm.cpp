/*
 * File:   getCommentForm.cpp
 * Author: bx830
 *
 * Created on 28 August 2015, 5:48 PM
 */

#include "getCommentForm.h"
#include <qt4/Qt/qmessagebox.h>

getCommentForm::getCommentForm() {
    widget.setupUi(this);
    connect(widget.okButton, SIGNAL(clicked()), this, SLOT(saveComment()));
    connect(widget.cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

getCommentForm::~getCommentForm() {
}

void getCommentForm::saveComment(){
    string comment = widget.textEdit->toPlainText().toStdString();
    if (comment.empty())
    {
        QMessageBox message;
        message.setWindowTitle("Error");
        message.setText("The file has NOT been modified,no save required");
        message.setStandardButtons(QMessageBox::Yes);
        message.setDefaultButton(QMessageBox::Yes);
        message.exec();
    }
    else
    {
        commentText = comment;
        setResult(OK);
        accept();
    }
}

string getCommentForm::getComment()
{
    return commentText;
}
void getCommentForm::cancel()
{
   setResult(CANCEL);
   accept();
} 