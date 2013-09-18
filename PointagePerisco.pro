#-------------------------------------------------
#
# Project created by QtCreator 2013-04-16T13:38:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PointagePerisco
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pointage.cpp \
    presence.cpp \
    file.cpp \
    listing.cpp

HEADERS  += mainwindow.h \
    pointage.h \
    presence.h \
    file.h \
    listing.h

FORMS    += mainwindow.ui \
    listing.ui
