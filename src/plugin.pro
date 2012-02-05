!win32 { error("sorry, only win32 supported") }

QT += network
#QT -= gui
TARGET = plugin_AeroSIMRC
TEMPLATE = lib

# disable depend of MSVRT*.dll
win32-msvc* {
    QMAKE_CXXFLAGS_RELEASE -= -MD
    QMAKE_CXXFLAGS_MT_DLL += -MT
}

# input <<<<<<
HEADERS += \
    aerosimdatastruct.h \
    enums.h \
    plugin.h \
    qdebughandler.h \
    udpconnect.h \
    settings.h \
    osd.h

SOURCES += \
    qdebughandler.cpp \
    plugin.cpp \
    udpconnect.cpp \
    settings.cpp \
    osd.cpp

# output >>>>>>
DLLDESTDIR = ../CopterControl
#DLLDESTDIR = e:/sims/AeroSIM-RC/Plugin/CopterControl

RESOURCES += \
    ../share/images.qrc

