QT += core
QT -= gui

CONFIG += c++11

TARGET = pcobarrier
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
        hoaremonitor.cpp

HEADERS += \
    pcobarrier.h \
hoaremonitor.h
