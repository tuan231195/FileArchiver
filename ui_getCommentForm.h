/********************************************************************************
** Form generated from reading UI file 'getCommentForm.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETCOMMENTFORM_H
#define UI_GETCOMMENTFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_getCommentForm
{
public:
    QLabel *label;
    QTextEdit *textEdit;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *getCommentForm)
    {
        if (getCommentForm->objectName().isEmpty())
            getCommentForm->setObjectName(QString::fromUtf8("getCommentForm"));
        getCommentForm->resize(400, 300);
        label = new QLabel(getCommentForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 251, 41));
        textEdit = new QTextEdit(getCommentForm);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(30, 80, 331, 131));
        cancelButton = new QPushButton(getCommentForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(170, 240, 98, 27));
        okButton = new QPushButton(getCommentForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(270, 240, 98, 27));

        retranslateUi(getCommentForm);

        QMetaObject::connectSlotsByName(getCommentForm);
    } // setupUi

    void retranslateUi(QDialog *getCommentForm)
    {
        getCommentForm->setWindowTitle(QApplication::translate("getCommentForm", "getCommentForm", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("getCommentForm", "Comment to go with version of file", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("getCommentForm", "cancel", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("getCommentForm", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class getCommentForm: public Ui_getCommentForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETCOMMENTFORM_H
