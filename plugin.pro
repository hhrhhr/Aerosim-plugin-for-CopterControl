TEMPLATE  = subdirs
CONFIG   += warn_on #ordered

SUBDIRS += plugin tools copydata

plugin.file         = src/plugin.pro
tools.file          = src/tools.pro
copydata.file       = copydata.pri

OTHER_FILES += \
    share/plugin.txt \
    share/cc_plugin.ini \
    share/cc_on.tga \
    share/cc_off.tga \
    share/cc_on_hover.tga \
    share/cc_off_hover.tga
