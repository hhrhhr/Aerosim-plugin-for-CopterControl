TEMPLATE = subdirs

RES_DIR = $${PWD}/share
CC_DIR = CopterControl

#uncomment this line to copy plugin files to the plugin directory
#SIM_DIR = e:\\sims\\AeroSIM-RC

FILES = \
    $${RES_DIR}/cc_off.tga \
    $${RES_DIR}/cc_on.tga \
    $${RES_DIR}/cc_off_hover.tga \
    $${RES_DIR}/cc_on_hover.tga \
    $${RES_DIR}/cc_plugin.ini \
    $${RES_DIR}/plugin.txt

FILES_WIN = $${FILES}
FILES_WIN ~= s,/,\\,g
DEST_DIR_WIN = $${OUT_PWD}/$${CC_DIR}/
DEST_DIR_WIN ~= s,/,\\,g

for(file, FILES_WIN) {
    copydata.commands += $(COPY_FILE) $${file} "$${DEST_DIR_WIN}" $$escape_expand(\\n\\t)
}
!isEmpty(SIM_DIR) {
    copydata.commands += xcopy /y /i $$CC_DIR $$SIM_DIR\\Plugin\\$$CC_DIR
}

copydata.target = FORCE
QMAKE_EXTRA_TARGETS += copydata
