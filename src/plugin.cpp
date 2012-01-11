#include "plugin.h"
//#include "udpconnect.h"

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h>

// Used to initialise menu checkboxes showing OSD with HUD
bool isFirstRun = TRUE;

// static to remain in memory when function exits
QString debugInfo;
QString pluginFolder;
QString outputFolder;

/*// Qt variable begin
QFile plglog;
QTime blueLedTimer;
int blueLedTimeout = 1000;
qreal channel[6] = {0,0,0,0,0,0};
quint32 packetCounter = 0;
qint32 errors = 0;
// Qt variable end
*/

extern "C" int __stdcall DllMain(void * /*hinstDLL*/, quint32 fdwReason, void * /*lpvReserved*/)
{
    switch (fdwReason) {
    case 0: //DLL_PROCESS_DETACH:
        // free resources
//        delete udp;
        break;
    case 1: //DLL_PROCESS_ATTACH:
        break;
    case 2: //DLL_THREAD_ATTACH:
        break;
    case 3: //DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(quint32 *sizeSimToPlugin,
                                                       quint32 *sizePluginToSim,
                                                       quint32 *sizePluginInit)
{
    *sizeSimToPlugin = sizeof(simToPlugin);
    *sizePluginToSim = sizeof(pluginToSim);
    *sizePluginInit  = sizeof(pluginInit);
}

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(pluginInit *p)
{
    debugInfo.reserve(4096);
    pluginFolder.reserve(MAX_PATH);
    outputFolder.reserve(MAX_PATH);
    pluginFolder = p->strPluginFolder;
    outputFolder = p->strOutputFolder;
}

//-----------------------------------------------------------------------------

void Run_Command_Reset(/*const simToPlugin *stp,
                             pluginToSim *pts*/)
{
    // Print some debug info, although it will only be seen during one frame
    debugInfo.append("\nRESET");
}

void Run_BlinkLEDs(/*const simToPlugin *stp,*/
                         pluginToSim *pts,
                   bool &isEnable)
{
    if(isEnable) {
        pts->newMenuStatus |= MenuLedBlue;
    } else {
        pts->newMenuStatus = 0;
    }
}

void InfoText(const simToPlugin *stp,
                    pluginToSim *pts)
{
    debugInfo.append(
                QString(
                    "Plugin Folder = %1\n"
                    "Output Folder = %2\n"
                    "nStructSize = %3  "
                    "fIntegrationTimeStep = %4\n"
                    "\n"
                    "Aileron   TX = %5  RX = %6  RCMD TX = %7  RX = %8\n"
                    "Elevator  TX = %9  RX = %10  RCMD TX = %11  RX = %12\n"
                    "Throttle  TX = %13  RX = %14  RCMD TX = %15  RX = %16\n"
                    "Rudder    TX = %17  RX = %18  RCMD TX = %19  RX = %20\n"
                    "Channel5  TX = %21  RX = %22  RCMD TX = %23  RX = %24\n"
                    "Channel6  TX = %25  RX = %26  RCMD TX = %27  RX = %28\n"
                    "Channel7  TX = %29  RX = %30  RCMD TX = %31  RX = %32\n"
                    "PluginCh1 TX = %33  RX = %34  RCMD TX = %35  RX = %36\n"
                    "PluginCh2 TX = %37  RX = %38  RCMD TX = %39  RX = %40\n"
                    "FPVCamPan TX = %41  RX = %42  RCMD TX = %43  RX = %44\n"
                    "FPVCamTil TX = %45  RX = %46  RCMD TX = %47  RX = %48\n"
                    "\n"
                    "MenuItems = %49\n"
                    )
                .arg(pluginFolder)
                .arg(outputFolder)
                .arg(stp->structSize)
                .arg(1.0 / stp->simTimeStep, 4, 'f', 1)
                .arg(stp->chSimTX[ChAileron], 5, 'f', 2)
                .arg(stp->chSimRX[ChAileron], 5, 'f', 2)
                .arg(pts->chNewTX[ChAileron], 5, 'f', 2)
                .arg(pts->chNewRX[ChAileron], 5, 'f', 2)
                .arg(stp->chSimTX[ChElevator], 5, 'f', 2)
                .arg(stp->chSimRX[ChElevator], 5, 'f', 2)
                .arg(pts->chNewTX[ChElevator], 5, 'f', 2)
                .arg(pts->chNewRX[ChElevator], 5, 'f', 2)
                .arg(stp->chSimTX[ChThrottle], 5, 'f', 2)
                .arg(stp->chSimRX[ChThrottle], 5, 'f', 2)
                .arg(pts->chNewTX[ChThrottle], 5, 'f', 2)
                .arg(pts->chNewRX[ChThrottle], 5, 'f', 2)
                .arg(stp->chSimTX[ChRudder], 5, 'f', 2)
                .arg(stp->chSimRX[ChRudder], 5, 'f', 2)
                .arg(pts->chNewTX[ChRudder], 5, 'f', 2)
                .arg(pts->chNewRX[ChRudder], 5, 'f', 2)
                .arg(stp->chSimTX[Ch5], 5, 'f', 2)
                .arg(stp->chSimRX[Ch5], 5, 'f', 2)
                .arg(pts->chNewTX[Ch5], 5, 'f', 2)
                .arg(pts->chNewRX[Ch5], 5, 'f', 2)
                .arg(stp->chSimTX[Ch6], 5, 'f', 2)
                .arg(stp->chSimRX[Ch6], 5, 'f', 2)
                .arg(pts->chNewTX[Ch6], 5, 'f', 2)
                .arg(pts->chNewRX[Ch6], 5, 'f', 2)
                .arg(stp->chSimTX[Ch7], 5, 'f', 2)
                .arg(stp->chSimRX[Ch7], 5, 'f', 2)
                .arg(pts->chNewTX[Ch7], 5, 'f', 2)
                .arg(pts->chNewRX[Ch7], 5, 'f', 2)
                .arg(stp->chSimTX[ChPlugin1], 5, 'f', 2)
                .arg(stp->chSimRX[ChPlugin1], 5, 'f', 2)
                .arg(pts->chNewTX[ChPlugin1], 5, 'f', 2)
                .arg(pts->chNewRX[ChPlugin1], 5, 'f', 2)
                .arg(stp->chSimTX[ChPlugin2], 5, 'f', 2)
                .arg(stp->chSimRX[ChPlugin2], 5, 'f', 2)
                .arg(pts->chNewTX[ChPlugin2], 5, 'f', 2)
                .arg(pts->chNewRX[ChPlugin2], 5, 'f', 2)
                .arg(stp->chSimTX[ChFpvPan], 5, 'f', 2)
                .arg(stp->chSimRX[ChFpvPan], 5, 'f', 2)
                .arg(pts->chNewTX[ChFpvPan], 5, 'f', 2)
                .arg(pts->chNewRX[ChFpvPan], 5, 'f', 2)
                .arg(stp->chSimTX[ChFpvTilt], 5, 'f', 2)
                .arg(stp->chSimRX[ChFpvTilt], 5, 'f', 2)
                .arg(pts->chNewTX[ChFpvTilt], 5, 'f', 2)
                .arg(pts->chNewRX[ChFpvTilt], 5, 'f', 2)
                .arg(stp->simMenuStatus)
    );
}


SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(const simToPlugin *stp,
                                               pluginToSim *pts)
{
    debugInfo = "---\n";
    // By default do not change the Menu Items of type CheckBox
    pts->newMenuStatus = stp->simMenuStatus;

    // Extract Menu Commands from Flags
    bool isReset = (stp->simMenuStatus & MenuCmdReset) != 0;
    bool isEnable = (stp->simMenuStatus & MenuEnable) != 0;
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
    pts->dbgInfoText = debugInfo.toAscii();
    isFirstRun = FALSE;
}
