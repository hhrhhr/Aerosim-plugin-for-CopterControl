TEMPLATE = subdirs

#uncomment next line to copy plugin files to the simulator directory
#SIM_DIR = e:\\sims\\AeroSIM-RC
RES_DIR = $${PWD}/share
CC_DIR = CopterControl

FILES = \
    $${RES_DIR}/cc_off.tga \
    $${RES_DIR}/cc_on.tga \
    $${RES_DIR}/cc_off_hover.tga \
    $${RES_DIR}/cc_on_hover.tga \
    $${RES_DIR}/cc_plugin.ini \
    $${RES_DIR}/plugin.txt

FILES_WIN = $${FILES}
FILES_WIN ~= s,/,\\,g
DEST_DIR_WIN = $${OUT_PWD}/$${CC_DIR}
DEST_DIR_WIN ~= s,/,\\,g

#used QMAKE_COPY_DIR (xcopy) instead QMAKE_COPY_FILES (copy)
for(file, FILES_WIN) {
    copydata.commands += $(COPY_DIR) $${file} $${DEST_DIR_WIN}\\ $$escape_expand(\\n\\t)
}

!isEmpty(SIM_DIR) {
    SIM_DIR_WIN = $${SIM_DIR}
    SIM_DIR_WIN ~= s,/,\\,g

    copydata.commands += $(COPY_DIR) $${DEST_DIR_WIN} \
                         $${SIM_DIR_WIN}\\Plugin\\$${CC_DIR}\\ $$escape_expand(\\n\\t)

    QTLIBS = \
        $$[QT_INSTALL_BINS]/QtCore4.dll \
        $$[QT_INSTALL_BINS]/QtNetwork4.dll
    win32-g++{
        QTLIBS += $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll
        QTLIBS += $$[QT_INSTALL_BINS]/mingwm10.dll
    }

    QTLIBS_WIN = $${QTLIBS}
    QTLIBS_WIN ~= s,/,\\,g

    for(qtlibs, QTLIBS_WIN) {
        copydata.commands += $(COPY_DIR) $${qtlibs} $${SIM_DIR_WIN}\\ $$escape_expand(\\n\\t)
    }

    copydata.commands += pushd $${SIM_DIR_WIN} && attrib -a -r -s -h -i /s /d && popd
}

message($$copydata.commands)

copydata.target = FORCE
QMAKE_EXTRA_TARGETS += copydata
