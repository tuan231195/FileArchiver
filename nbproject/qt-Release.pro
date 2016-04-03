# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = Assignment1
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui widgets
SOURCES += Database.cpp FileArchiver.cpp FileRecord.cpp MyTableModel.cpp MyWindow.cpp RetrieveForm.cpp VersionRec.cpp getCommentForm.cpp main.cpp
HEADERS += Database.h FileArchiver.h FileRecord.h MyTableModel.h MyWindow.h RetrieveForm.h Utils.h VersionInfo.h VersionRec.h getCommentForm.h
FORMS += MyWindow.ui RetrieveForm.ui getCommentForm.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
