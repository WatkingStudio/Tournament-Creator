#-------------------------------------------------
#
# Project created by QtCreator 2018-12-18T14:50:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tournament_Creator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=  src/main.cpp \
    src/mainwindow.cpp \
    src/startpage.cpp \
    src/util.cpp \
    src/tournamentcreator.cpp \
    src/player.cpp \
    src/matchupspage.cpp \
    src/resultspage.cpp \
    src/directmatchupswap.cpp

HEADERS +=  include/mainwindow.h \
    include/util.h \
    include/player.h \
    include/directmatchupswap.h

FORMS +=  forms/mainwindow.ui \
    forms/directmatchupswap.ui

RESOURCES += \
    resources/resourses.qrc
