!win32 { error("Sorry, only win32 supported") }

QT       += network
QT       -= gui

TARGET = plugin_AeroSIMRC
TEMPLATE = lib

DEPENDPATH += .
INCLUDEPATH += .



### input ###
HEADERS += \
    datafromaerosim.h \
    datatoaerosim.h \
    plugininit.h \
    plugin_AeroSIMRC.h

SOURCES += \
    plugin_AeroSIMRC.cpp

### output ###
DLLDESTDIR = CopterControl
#DLLDESTDIR = e:/sims/AeroSIM-RC/Plugin/CopterControl

#include(copydata.pri)

