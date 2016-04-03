/********************************************************************************
** Form generated from reading UI file 'MyWindows.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWINDOWS_H
#define UI_MYWINDOWS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_MyWindows
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MyWindows)
    {
        if (MyWindows->objectName().isEmpty())
            MyWindows->setObjectName(QString::fromUtf8("MyWindows"));
        MyWindows->resize(400, 300);
        buttonBox = new QDialogButtonBox(MyWindows);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        retranslateUi(MyWindows);
        QObject::connect(buttonBox, SIGNAL(accepted()), MyWindows, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MyWindows, SLOT(reject()));

        QMetaObject::connectSlotsByName(MyWindows);
    } // setupUi

    void retranslateUi(QDialog *MyWindows)
    {
        MyWindows->setWindowTitle(QApplication::translate("MyWindows", "MyWindows", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MyWindows: public Ui_MyWindows {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWINDOWS_H
