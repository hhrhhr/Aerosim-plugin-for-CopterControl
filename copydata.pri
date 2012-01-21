TEMPLATE = subdirs

RES_DIR = $${PWD}/share
CC_DIR = CopterControl

FILES = \
    $${RES_DIR}/cc_off.tga \
    $${RES_DIR}/cc_on.tga \
    $${RES_DIR}/cc_plugin.ini \
    $${RES_DIR}/plugin.txt

    FILES_WIN = $${FILES}
    FILES_WIN ~= s,/,\\,g
    DEST_DIR_WIN = $${OUT_PWD}/$${CC_DIR}/
    DEST_DIR_WIN ~= s,/,\\,g
    for(file, FILES_WIN) {
        system(xcopy /y $${file} $${DEST_DIR_WIN})
    }
