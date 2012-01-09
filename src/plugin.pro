!win32 { error("sorry, only win32 supported") }

QT  += network
QT  -= gui

TARGET      = plugin_AeroSIMRC
TEMPLATE    = lib

# input <<<

HEADERS += \
    datafromaerosim.h \
    datatoaerosim.h \
    plugininit.h \
    plugin.h

SOURCES += \
    plugin.cpp

# output >>>

DLLDESTDIR = ../CopterControl
#DLLDESTDIR = e:/sims/AeroSIM-RC/Plugin/CopterControl

