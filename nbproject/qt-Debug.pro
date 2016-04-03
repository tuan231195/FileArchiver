# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = Assignment1
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets
SOURCES += Database.cpp FileArchiver.cpp FileRecord.cpp MyTableModel.cpp MyWindow.cpp RetrieveForm.cpp VersionRec.cpp getCommentForm.cpp main.cpp
HEADERS += Database.h FileArchiver.h FileRecord.h MyTableModel.h MyWindow.h RetrieveForm.h Utils.h VersionInfo.h VersionRec.h getCommentForm.h
FORMS += MyWindow.ui RetrieveForm.ui getCommentForm.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += /usr/include 
LIBS += -lmysqlcppconn  
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
