#-------------------------------------------------
#
# Project created by QtCreator 2012-02-20T22:12:47
#
#-------------------------------------------------

QT       += core gui

TARGET = qmous
TEMPLATE = app

INCLUDEPATH += ../../sdk ../../core
LIBS += -L./ -Wl,-rpath,./ -lMousCore

SOURCES += main.cpp\
        MainWindow.cpp \
    sqt/BrowserStyleTabWidget.cpp

HEADERS  += MainWindow.h \
    sqt/BrowserStyleTabWidget.h

FORMS    += MainWindow.ui

RESOURCES += \
    allres.qrc