#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T10:34:44
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = udp_chat_server
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.hpp
