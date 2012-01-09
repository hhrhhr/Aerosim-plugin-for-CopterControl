QT       += core gui network

TARGET   = udp_test
TEMPLATE = app

### input ###

SOURCES += udptestmain.cpp \
           udptestwidget.cpp

HEADERS += udptestwidget.h

FORMS   += udptestwidget.ui

### output ###

DESTDIR = ../tools
