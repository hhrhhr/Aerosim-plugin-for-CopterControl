TEMPLATE = subdirs

FILES = cc_off.tga cc_on.tga cc_plugin.ini plugin.txt

for(file, FILES) {
    !exists(CopterControl/) {
        system(mkdir CopterControl)
    }
    !exists(CopterControl/$$file) {
        FROM = $$PWD/share/$$file
        FROM ~= s|/|\\|g
        TO = $$OUT_PWD/CopterControl
        TO ~= s|/|\\|g
        system(copy \"$$FROM\" \"$$TO\" > nul)
    }
}
