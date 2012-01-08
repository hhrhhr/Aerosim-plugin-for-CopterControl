#ifndef PLUGININIT_H
#define PLUGININIT_H

// OBSOLETE. These data is now specified in plugin.txt
// Was used for customising the Top Bar Menu

// OBSOLETE. Command Menu Item. Click once to execute command.
#define OBSOLETE_MIT_COMMAND    (1 << 0)

// OBSOLETE. Menu Item with checkbox. Click to check/uncheck box.
#define OBSOLETE_MIT_CHECKBOX   (1 << 1)

// OBSOLETE. Line used to separate menu items into groups (placed below menu item)
#define OBSOLETE_MIT_SEPARATOR  (1 << 7)

struct TPluginMenuItem
{
    // OBSOLETE. Was Menu Item Type. Valid values are MIT_xxx defined above. This is now specified in plugin.txt
    unsigned long OBSOLETE_eType;

    // OBSOLETE. Was NULL or "" for menu item not in use. This is now specified in plugin.txt
    char *OBSOLETE_strName;
};

#define MAX_DLL_USER_MENU_ITEMS 16

struct TPluginInit
{
    // Size in bytes of TPluginMenu
    unsigned long nStructSize;

    // OBSOLETE. Was custom menu title. This is now specified in plugin.txt
    char *OBSOLETE_strMenuTitle;

    // OBSOLETE. Was custom menu items. This is now specified in plugin.txt
    TPluginMenuItem OBSOLETE_atMenuItem[MAX_DLL_USER_MENU_ITEMS];

    // Path to the plugin folder
    const char *strPluginFolder;

    // Writeable folder where new files can be created (to write configuration data, etc.)
    const char *strOutputFolder;
};

#endif // PLUGININIT_H
