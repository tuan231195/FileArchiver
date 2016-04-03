/********************************************************************************
** Form generated from reading UI file 'Mywindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWINDOW_H
#define UI_MYWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Mywindow
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Mywindow)
    {
        if (Mywindow->objectName().isEmpty())
            Mywindow->setObjectName(QString::fromUtf8("Mywindow"));
        Mywindow->resize(400, 300);
        buttonBox = new QDialogButtonBox(Mywindow);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        retranslateUi(Mywindow);
        QObject::connect(buttonBox, SIGNAL(accepted()), Mywindow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Mywindow, SLOT(reject()));

        QMetaObject::connectSlotsByName(Mywindow);
    } // setupUi

    void retranslateUi(QDialog *Mywindow)
    {
        Mywindow->setWindowTitle(QApplication::translate("Mywindow", "Mywindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Mywindow: public Ui_Mywindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWINDOW_H
