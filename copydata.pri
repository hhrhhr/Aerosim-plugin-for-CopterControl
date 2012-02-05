TEMPLATE = subdirs

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

    copydata.commands += -$(MKDIR) $${DEST_DIR_WIN} $$escape_expand(\\n\\t)

    for(file, FILES_WIN) {
        copydata.commands += $(COPY_FILE) $${file} "$${DEST_DIR_WIN}" $$escape_expand(\\n\\t)
    }

    # Edit copydata.cmd and use this to copy plugin files to the plugin directory
    copydata.commands += -cmd /c copydata.cmd $$escape_expand(\\n\\t)

    copydata.target = FORCE
    QMAKE_EXTRA_TARGETS += copydata
