/*
 * File:   main.cpp
 * Author: mz430
 *
 * Created on 19 August 2015, 3:23 PM
 */

#include <QApplication>
#include "MyWindow.h"
#include "iostream"
#include <fstream>
#include "MyRecord.h"
#include "MyTableModel.h"
#include "getCommentForm.h"
#include <qt4/QtGui/QApplication>
#include <qt4/QtCore/qtimer.h>
#include <qt4/QtGui/qwidget.h>


int main(int argc, char *argv[]) {


    QApplication app(argc, argv);
    MyWindow win;
   
    QPixmap pixmap("welcome.png");
    QSplashScreen splash(pixmap);
    splash.show();
   
    QTimer::singleShot(2000, &splash, SLOT(hide()));
    QTimer::singleShot(1500, &win, SLOT(show()));
    win.move ((QApplication::desktop()->width()-win.width())/2, (QApplication::desktop()-> height()-win.height())/2);
   
    return app.exec();

  
}
