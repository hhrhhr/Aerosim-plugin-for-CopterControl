#ifndef PLUGININIT_H
#define PLUGININIT_H

#define OBSOLETE_MIT_COMMAND    (1 << 0)
#define OBSOLETE_MIT_CHECKBOX   (1 << 1)
#define OBSOLETE_MIT_SEPARATOR  (1 << 7)

struct TPluginMenuItem
{
    unsigned long OBSOLETE_eType;
    char *OBSOLETE_strName;
}__attribute__((packed));

#define MAX_DLL_USER_MENU_ITEMS 16

struct pluginInit
{
    unsigned long nStructSize;
    char *OBSOLETE_strMenuTitle;
    TPluginMenuItem OBSOLETE_atMenuItem[MAX_DLL_USER_MENU_ITEMS];
    const char *strPluginFolder;
    const char *strOutputFolder;
}__attribute__((packed));

// normal - 144
// packed - 144

#endif // PLUGININIT_H
