/********************************************************************************
** Form generated from reading UI file 'RetrieveForm.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETRIEVEFORM_H
#define UI_RETRIEVEFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RetrieveForm
{
public:
    QLabel *directoryLabel;
    QLabel *filenameLabel;
    QLineEdit *directory;
    QLineEdit *filename;
    QPushButton *selectButton;
    QPushButton *cancel_button;
    QPushButton *ok_button;

    void setupUi(QDialog *RetrieveForm)
    {
        if (RetrieveForm->objectName().isEmpty())
            RetrieveForm->setObjectName(QString::fromUtf8("RetrieveForm"));
        RetrieveForm->resize(620, 410);
        directoryLabel = new QLabel(RetrieveForm);
        directoryLabel->setObjectName(QString::fromUtf8("directoryLabel"));
        directoryLabel->setGeometry(QRect(30, 50, 81, 31));
        filenameLabel = new QLabel(RetrieveForm);
        filenameLabel->setObjectName(QString::fromUtf8("filenameLabel"));
        filenameLabel->setGeometry(QRect(30, 130, 81, 31));
        directory = new QLineEdit(RetrieveForm);
        directory->setObjectName(QString::fromUtf8("directory"));
        directory->setEnabled(false);
        directory->setGeometry(QRect(110, 50, 361, 31));
        filename = new QLineEdit(RetrieveForm);
        filename->setObjectName(QString::fromUtf8("filename"));
        filename->setGeometry(QRect(110, 130, 261, 27));
        selectButton = new QPushButton(RetrieveForm);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setGeometry(QRect(500, 50, 81, 31));
        cancel_button = new QPushButton(RetrieveForm);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
        cancel_button->setGeometry(QRect(380, 310, 98, 27));
        ok_button = new QPushButton(RetrieveForm);
        ok_button->setObjectName(QString::fromUtf8("ok_button"));
        ok_button->setGeometry(QRect(500, 310, 98, 27));

        retranslateUi(RetrieveForm);

        QMetaObject::connectSlotsByName(RetrieveForm);
    } // setupUi

    void retranslateUi(QDialog *RetrieveForm)
    {
        RetrieveForm->setWindowTitle(QApplication::translate("RetrieveForm", "RetrieveForm", 0, QApplication::UnicodeUTF8));
        directoryLabel->setText(QApplication::translate("RetrieveForm", "Directory", 0, QApplication::UnicodeUTF8));
        filenameLabel->setText(QApplication::translate("RetrieveForm", "Filename", 0, QApplication::UnicodeUTF8));
        selectButton->setText(QApplication::translate("RetrieveForm", "Select", 0, QApplication::UnicodeUTF8));
        cancel_button->setText(QApplication::translate("RetrieveForm", "Cancel", 0, QApplication::UnicodeUTF8));
        ok_button->setText(QApplication::translate("RetrieveForm", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RetrieveForm: public Ui_RetrieveForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETRIEVEFORM_H
