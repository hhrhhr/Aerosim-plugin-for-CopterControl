#include "plugin.h"
#include "udpconnect.h"
#include "qdebughandler.h"
#include "enums.h"
#include "settings.h"

bool isFirstRun = true;
QString debugInfo(DBG_BUFFER_MAX_SIZE, ' ');
QString pluginFolder(MAX_PATH, ' ');
QString outputFolder(MAX_PATH, ' ');

QTime ledTimer;

UdpSender *sndr;
UdpReciever *rcvr;

const float RAD2DEG = 180.f / M_PI;
const float DEG2RAD = M_PI / 180.f;

//extern "C" int __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
extern "C" int __stdcall DllMain(void*, quint32 fdwReason, void*)
{
    switch (fdwReason) {
    case 0:
//        qDebug() << hinstDLL << "DLL_PROCESS_DETACH " << lpvReserved;
// free resources here
        rcvr->stop();
        rcvr->wait(500);
        delete rcvr;
        delete sndr;
        qDebug("------");
        break;
    case 1:
//        qDebug() << hinstDLL << " DLL_PROCESS_ATTACH " << lpvReserved;
        break;
    case 2:
//        qDebug() << hinstDLL << "DLL_THREAD_ATTACH " << lpvReserved;
        break;
    case 3:
//        qDebug() << hinstDLL << "DLL_THREAD_DETACH " << lpvReserved;
        break;
    }
    return true;
}

SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(quint32 *sizeSimToPlugin,
                                                       quint32 *sizePluginToSim,
                                                       quint32 *sizePluginInit)
{
    // debug redirection
    qInstallMsgHandler(myQDebugHandler);

    qDebug() << "AeroSIMRC_Plugin_ReportStructSizes";
    *sizeSimToPlugin = sizeof(simToPlugin);
    *sizePluginToSim = sizeof(pluginToSim);
    *sizePluginInit  = sizeof(pluginInit);
    qDebug() << "sizeSimToPlugin = " << *sizeSimToPlugin;
    qDebug() << "sizePluginToSim = " << *sizePluginToSim;
    qDebug() << "sizePluginInit  = " << *sizePluginInit;
}

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(pluginInit *p)
{
    qDebug() << "AeroSIMRC_Plugin_Init begin";

    pluginFolder = p->strPluginFolder;
    outputFolder = p->strOutputFolder;

    ledTimer.restart();

    Settings *ini = new Settings(pluginFolder);
    ini->read();

    sndr = new UdpSender(ini->getOutputMap(), ini->isFromTX());
    sndr->init(ini->remoteHost(), ini->remotePort());

    rcvr = new UdpReciever(ini->getInputMap(), ini->isToRX());
    rcvr->init(ini->localHost(), ini->localPort());

    // run thread
    rcvr->start();

    delete ini;
    qDebug() << "AeroSIMRC_Plugin_Init done";
}

//-----------------------------------------------------------------------------

void Run_Command_Reset(/*const simToPlugin *stp,
                               pluginToSim *pts*/)
{
    // Print some debug info, although it will only be seen during one frame
    debugInfo.append("\nRESET");
}

void Run_Command_WindowSizeAndPos(const simToPlugin *stp,
                                        pluginToSim *pts)
{
    static quint8 snSequence = 0;
    // half size
    qint16 width = stp->screenW / 2;
    qint16 height = stp->screenH / 2;

    switch(snSequence) {
    case 0:     // top left
        pts->newScreenX = 0;
        pts->newScreenY = 0;
        pts->newScreenW = width;
        pts->newScreenH = height;
        break;
    case 1:     // top right
        pts->newScreenX = width;
        pts->newScreenY = 0;
        pts->newScreenW = width;
        pts->newScreenH = height;
        break;
    case 2:     // bottom right
        pts->newScreenX = width;
        pts->newScreenY = height;
        pts->newScreenW = width;
        pts->newScreenH = height;
        break;
    case 3:     // bottom left
        pts->newScreenX = 0;
        pts->newScreenY = height;
        pts->newScreenW = width;
        pts->newScreenH = height;
        break;
    case 4:     // fullscreen
        pts->newScreenX = 0;
        pts->newScreenY = 0;
        pts->newScreenW = stp->screenW;
        pts->newScreenH = stp->screenH;
        break;
    default:    // something wrong
        qFatal("Run_Command_WindowSizeAndPos switch error");
    }

    snSequence++;
    if(snSequence > 4)
        snSequence = 0;
}

void Run_BlinkLEDs(const simToPlugin *stp,
                         pluginToSim *pts)
{
    if ((stp->simMenuStatus & MenuEnable) != 0) {
        pts->newMenuStatus |= MenuLedGreen;
        int timeout;
        if (rcvr->getArmed() == 0)
            timeout = 1000;
        else if (rcvr->getArmed() == 1)
            timeout = 33;
        else if (rcvr->getArmed() == 2)
            timeout = 100;
        else
            timeout = 2000;
        if (ledTimer.elapsed() > timeout) {
            ledTimer.restart();
            pts->newMenuStatus ^= MenuLedBlue;
        }
        quint8 mode = rcvr->getMode();
        if (mode == 4) {
            pts->newMenuStatus |= MenuFMode3;
            pts->newMenuStatus |= MenuFMode2;
            pts->newMenuStatus |= MenuFMode1;
        } else if (mode == 4) {
            pts->newMenuStatus |= MenuFMode3;
            pts->newMenuStatus |= MenuFMode2;
            pts->newMenuStatus &= ~MenuFMode1;
        } else if (mode == 3) {
            pts->newMenuStatus |= MenuFMode3;
            pts->newMenuStatus &= ~MenuFMode2;
            pts->newMenuStatus |= MenuFMode1;
        } else if (mode == 2) {
            pts->newMenuStatus |= MenuFMode3;
            pts->newMenuStatus &= ~MenuFMode2;
            pts->newMenuStatus &= ~MenuFMode1;
        } else if (mode == 1) {
            pts->newMenuStatus &= ~MenuFMode3;
            pts->newMenuStatus |= MenuFMode2;
            pts->newMenuStatus &= ~MenuFMode1;
        } else if (mode == 0) {
            pts->newMenuStatus &= ~MenuFMode3;
            pts->newMenuStatus &= ~MenuFMode2;
            pts->newMenuStatus |= MenuFMode1;
        } else {
            pts->newMenuStatus &= ~MenuFMode3;
            pts->newMenuStatus &= ~MenuFMode2;
            pts->newMenuStatus &= ~MenuFMode1;
        }
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
                    // Model data
                    "\n"
                    "fPosX,Y,Z    = (%50, %51, %52)\n"
                    "fVelX,Y,Z    = (%53, %54, %55)\n"
                    "fAngVelX,Y,Z = (%56, %57, %58)\n"
                    "fAccelX,Y,Z  = (%59, %60, %61)\n"
                    "\n"
                    "Lat, Long    = %62, %63\n"
                    "fHeightAboveTerrain = %64\n"
                    "\n"
                    "fHeading = %65   fPitch = %66   fRoll = %67\n"
                    )
                .arg(pluginFolder)
                .arg(outputFolder)
                .arg(stp->structSize)
                .arg(1.0 / stp->simTimeStep, 4, 'f', 1)
                .arg(stp->chSimTX[Ch1Aileron], 5, 'f', 2)
                .arg(stp->chSimRX[Ch1Aileron], 5, 'f', 2)
                .arg(pts->chNewTX[Ch1Aileron], 5, 'f', 2)
                .arg(pts->chNewRX[Ch1Aileron], 5, 'f', 2)
                .arg(stp->chSimTX[Ch2Elevator], 5, 'f', 2)
                .arg(stp->chSimRX[Ch2Elevator], 5, 'f', 2)
                .arg(pts->chNewTX[Ch2Elevator], 5, 'f', 2)
                .arg(pts->chNewRX[Ch2Elevator], 5, 'f', 2)
                .arg(stp->chSimTX[Ch3Throttle], 5, 'f', 2)
                .arg(stp->chSimRX[Ch3Throttle], 5, 'f', 2)
                .arg(pts->chNewTX[Ch3Throttle], 5, 'f', 2)
                .arg(pts->chNewRX[Ch3Throttle], 5, 'f', 2)
                .arg(stp->chSimTX[Ch4Rudder], 5, 'f', 2)
                .arg(stp->chSimRX[Ch4Rudder], 5, 'f', 2)
                .arg(pts->chNewTX[Ch4Rudder], 5, 'f', 2)
                .arg(pts->chNewRX[Ch4Rudder], 5, 'f', 2)
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
                .arg(stp->chSimTX[Ch23Plugin1], 5, 'f', 2)
                .arg(stp->chSimRX[Ch23Plugin1], 5, 'f', 2)
                .arg(pts->chNewTX[Ch23Plugin1], 5, 'f', 2)
                .arg(pts->chNewRX[Ch23Plugin1], 5, 'f', 2)
                .arg(stp->chSimTX[Ch24Plugin2], 5, 'f', 2)
                .arg(stp->chSimRX[Ch24Plugin2], 5, 'f', 2)
                .arg(pts->chNewTX[Ch24Plugin2], 5, 'f', 2)
                .arg(pts->chNewRX[Ch24Plugin2], 5, 'f', 2)
                .arg(stp->chSimTX[Ch12FPVPan], 5, 'f', 2)
                .arg(stp->chSimRX[Ch12FPVPan], 5, 'f', 2)
                .arg(pts->chNewTX[Ch12FPVPan], 5, 'f', 2)
                .arg(pts->chNewRX[Ch12FPVPan], 5, 'f', 2)
                .arg(stp->chSimTX[Ch13FPVTilt], 5, 'f', 2)
                .arg(stp->chSimRX[Ch13FPVTilt], 5, 'f', 2)
                .arg(pts->chNewTX[Ch13FPVTilt], 5, 'f', 2)
                .arg(pts->chNewRX[Ch13FPVTilt], 5, 'f', 2)
                .arg(stp->simMenuStatus)
                .arg(stp->posX, 5, 'f', 2)
                .arg(stp->posY, 5, 'f', 2)
                .arg(stp->posZ, 5, 'f', 2)
                .arg(stp->velX, 5, 'f', 2)
                .arg(stp->velY, 5, 'f', 2)
                .arg(stp->velZ, 5, 'f', 2)
                .arg(stp->angVelXm, 5, 'f', 2)
                .arg(stp->angVelYm, 5, 'f', 2)
                .arg(stp->angVelZm, 5, 'f', 2)
                .arg(stp->accelXm, 5, 'f', 2)
                .arg(stp->accelYm, 5, 'f', 2)
                .arg(stp->accelZm, 5, 'f', 2)
                .arg(stp->latitude, 5, 'f', 2)
                .arg(stp->longitude, 5, 'f', 2)
                .arg(stp->AGL, 5, 'f', 2)
                .arg(stp->heading*RAD2DEG, 5, 'f', 2)
                .arg(stp->pitch*RAD2DEG, 5, 'f', 2)
                .arg(stp->roll*RAD2DEG, 5, 'f', 2)
    );
}

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(const simToPlugin *stp,
                                               pluginToSim *pts)
{
    debugInfo = "---\n";
    // By default do not change the Menu Items of type CheckBox
    pts->newMenuStatus = stp->simMenuStatus;
    // Extract Menu Commands from Flags
    bool isReset  = (stp->simMenuStatus & MenuCmdReset) != 0;
    bool isEnable = (stp->simMenuStatus & MenuEnable) != 0;
    bool isTxON   = (stp->simMenuStatus & MenuTx) != 0;
    bool isRxON   = (stp->simMenuStatus & MenuRx) != 0;
    bool isScreen = (stp->simMenuStatus & MenuScreen) != 0;
    // Run commands
    if (isReset) {
        Run_Command_Reset(/*stp, pts*/);
    } else if (isScreen) {
        Run_Command_WindowSizeAndPos(stp, pts);
    } else {
        Run_BlinkLEDs(stp, pts);
        if (isEnable) {
            if (isTxON)
                sndr->sendDatagram(stp);
            if (isRxON)
                rcvr->setChannels(pts);
        }

        // network lag
        debugInfo.append(QString("out: %1, inp: %2, delta: %3\n")
                         .arg(sndr->pcks() - 1)
                         .arg(rcvr->pcks())
                         .arg(sndr->pcks() - rcvr->pcks() - 1)
                         );
    }

    // debug info is shown on the screen
    InfoText(stp, pts);
    pts->dbgInfoText = debugInfo.toAscii();
    isFirstRun = false;
}
