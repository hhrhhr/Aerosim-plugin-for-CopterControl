#include "plugin.h"
//#include "udpconnect.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>

// Custom Menu Item masks
#define MENU_CMD_RESET  (1 << 0)
#define MENU_ENABLE     (1 << 1)
#define MENU_TX         (1 << 2)
#define MENU_RX         (1 << 3)
#define MENU_LED_BLUE   (1 << 4)
#define MENU_LED_GREEN  (1 << 5)

// Used to initialise menu checkboxes showing OSD with HUD
bool g_bFirstRun = true;

// static to remain in memory when function exits
const unsigned int dbg_sz = 4096;
char debugInfo[dbg_sz] = "";
unsigned int p_dbg = 0;

char pluginFolder[MAX_PATH];
char outputFolder[MAX_PATH];

/*// Qt variable begin
QFile plglog;
QTime blueLedTimer;
int blueLedTimeout = 1000;
qreal channel[6] = {0,0,0,0,0,0};
quint32 packetCounter = 0;
qint32 errors = 0;
// Qt variable end
*/

extern "C" BOOL APIENTRY DllMain(HANDLE /*hinstDLL*/, DWORD fdwReason, LPVOID /*lpvReserved*/)
{
    switch (fdwReason) {
    case DLL_PROCESS_DETACH:
        // free resources
//        delete udp;
        break;
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(unsigned long *sizeSimToPlugin,
                                                       unsigned long *sizePluginToSim,
                                                       unsigned long *sizePluginInit)
{
    *sizeSimToPlugin = sizeof(simToPlugin);
    *sizePluginToSim = sizeof(pluginToSim);
    *sizePluginInit  = sizeof(pluginInit);
}

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(pluginInit *p)
{
    strcpy_s(pluginFolder, p->strPluginFolder);
    strcpy_s(outputFolder, p->strOutputFolder);
}

//-----------------------------------------------------------------------------

void Run_Command_Reset(/*const simToPlugin *stp,
                             pluginToSim *pts*/)
{
    // Print some debug info, although it will only be seen during one frame
    p_dbg += sprintf_s(debugInfo + p_dbg, dbg_sz - p_dbg, "\nRESET");
}

void Run_BlinkLEDs(/*const simToPlugin *stp,*/
                         pluginToSim *pts,
                   bool &isEnable)
{
    if(isEnable) {
        pts->Menu_nFlags_MenuItem_New_CheckBox_Status |= MENU_LED_GREEN;
    } else {
        pts->Menu_nFlags_MenuItem_New_CheckBox_Status = 0;
    }
}

void InfoText(const simToPlugin *stp,
                    pluginToSim *pts)
{
    p_dbg += sprintf_s(debugInfo + p_dbg, dbg_sz - p_dbg,
            "Plugin Folder = %s\n"
            "Output Folder = %s\n"
            "nStructSize = %d"
            "fIntegrationTimeStep = %f\n"
            "\n"
            "Aileron   TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "Elevator  TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "Throttle  TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "Rudder    TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "Channel5  TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "Channel6  TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "Channel7  TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "PluginCh1 TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "PluginCh2 TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "FPVCamPan TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "FPVCamTil TX = % 4.2f  RX = % 4.2f  RCMD TX = % 4.2f  RX = % 4.2f\n"
            "\n"
            "MenuItems = %lu\n"
            "----------------------------------------------------------------------\n"
            ,pluginFolder
            ,outputFolder
            ,stp->nStructSize
            ,stp->Simulation_fIntegrationTimeStep
            ,stp->Channel_afValue_TX    [ch_aileron]
            ,stp->Channel_afValue_RX    [ch_aileron]
            ,pts->Channel_afNewValue_TX [ch_aileron]
            ,pts->Channel_afNewValue_RX [ch_aileron]
            ,stp->Channel_afValue_TX    [ch_elevator]
            ,stp->Channel_afValue_RX    [ch_elevator]
            ,pts->Channel_afNewValue_TX [ch_elevator]
            ,pts->Channel_afNewValue_RX [ch_elevator]
            ,stp->Channel_afValue_TX    [ch_throttle]
            ,stp->Channel_afValue_RX    [ch_throttle]
            ,pts->Channel_afNewValue_TX [ch_throttle]
            ,pts->Channel_afNewValue_RX [ch_throttle]
            ,stp->Channel_afValue_TX    [ch_rudder]
            ,stp->Channel_afValue_RX    [ch_rudder]
            ,pts->Channel_afNewValue_TX [ch_rudder]
            ,pts->Channel_afNewValue_RX [ch_rudder]
            ,stp->Channel_afValue_TX    [ch_5]
            ,stp->Channel_afValue_RX    [ch_5]
            ,pts->Channel_afNewValue_TX [ch_5]
            ,pts->Channel_afNewValue_RX [ch_5]
            ,stp->Channel_afValue_TX    [ch_6]
            ,stp->Channel_afValue_RX    [ch_6]
            ,pts->Channel_afNewValue_TX [ch_6]
            ,pts->Channel_afNewValue_RX [ch_6]
            ,stp->Channel_afValue_TX    [ch_7]
            ,stp->Channel_afValue_RX    [ch_7]
            ,pts->Channel_afNewValue_TX [ch_7]
            ,pts->Channel_afNewValue_RX [ch_7]
            ,stp->Channel_afValue_TX    [ch_plugin_1]
            ,stp->Channel_afValue_RX    [ch_plugin_1]
            ,pts->Channel_afNewValue_TX [ch_plugin_1]
            ,pts->Channel_afNewValue_RX [ch_plugin_1]
            ,stp->Channel_afValue_TX    [ch_plugin_2]
            ,stp->Channel_afValue_RX    [ch_plugin_2]
            ,pts->Channel_afNewValue_TX [ch_plugin_2]
            ,pts->Channel_afNewValue_RX [ch_plugin_2]
            ,stp->Channel_afValue_TX    [ch_fpv_pan]
            ,stp->Channel_afValue_RX    [ch_fpv_pan]
            ,pts->Channel_afNewValue_TX [ch_fpv_pan]
            ,pts->Channel_afNewValue_RX [ch_fpv_pan]
            ,stp->Channel_afValue_TX    [ch_fpv_tilt]
            ,stp->Channel_afValue_RX    [ch_fpv_tilt]
            ,pts->Channel_afNewValue_TX [ch_fpv_tilt]
            ,pts->Channel_afNewValue_RX [ch_fpv_tilt]

            ,stp->Menu_nFlags_MenuItem_Status
            );
}


SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(const simToPlugin *stp,
                                               pluginToSim *pts)
{
    // init debug string
    p_dbg = sprintf_s(debugInfo, dbg_sz, "---\n");
    // By default do not change the Menu Items of type CheckBox
    pts->Menu_nFlags_MenuItem_New_CheckBox_Status = stp->Menu_nFlags_MenuItem_Status;

    // Extract Menu Commands from Flags
    bool isReset = (stp->Menu_nFlags_MenuItem_Status & MENU_CMD_RESET) != 0;
    bool isEnable = (stp->Menu_nFlags_MenuItem_Status & MENU_ENABLE) != 0;
//    bool isTxON = (stp->Menu_nFlags_MenuItem_Status & MENU_TX) != 0;
//    bool isRxON = (stp->Menu_nFlags_MenuItem_Status & MENU_RX) != 0;

    // Run commands
    if(isReset) {
        Run_Command_Reset(/*stp, pts*/);
    } else {
        Run_BlinkLEDs(/*stp,*/ pts, isEnable);
    }

    // debug info is shown on the screen
    InfoText(stp, pts);
    pts->Debug_pucOnScreenInfoText = debugInfo;
    g_bFirstRun = false;
}
