#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T21:07:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = udp_chat_client
TEMPLATE = app

CONFIG   += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    client.hpp \
    ../message_interface.hpp

FORMS    += mainwindow.ui
