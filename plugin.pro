TEMPLATE  = subdirs
CONFIG   += warn_on #ordered

SUBDIRS += plugin tools share copydata

plugin.file         = src/plugin.pro
tools.file          = src/tools.pro
copydata.file       = copydata.pri
