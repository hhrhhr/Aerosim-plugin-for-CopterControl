#include "plugin_AeroSIMRC.h"

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include <math.h>

// Custom Menu Item masks
#define MASK_MENU_ITEM__COMMAND_RESET   (1 << 0)    // 1
#define MASK_MENU_ITEM__ENABLE          (1 << 1)    // 2
#define MASK_MENU_ITEM__TX              (1 << 2)    // 4
#define MASK_MENU_ITEM__RX              (1 << 3)    // 8
#define MASK_MENU_ITEM__LED_BLUE        (1 << 4)    // 16
#define MASK_MENU_ITEM__LED_GREEN       (1 << 5)    // 32

// Used to initialise menu checkboxes showing OSD with HUD
bool g_bFirstRun = true;

// static to remain in memory when function exits
char g_strDebugInfo[10000] = "";
char g_strPluginFolder[MAX_PATH];
char g_strOutputFolder[MAX_PATH];

// Qt variable begin
QTime blueLedTimer;
int blueLedTimeout = 1000;
QUdpSocket *inSocket = NULL;
QUdpSocket *outSocket = NULL;
QHostAddress outHost;
int outPort;
QFile dbglog;
// Qt variable end

// This function is already written for you. No need to modify it.
SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(unsigned long *pnSizeOf_TDataFromAeroSimRC,
                                                       unsigned long *pnSizeOf_TDataToAeroSimRC,
                                                       unsigned long *pnSizeOf_TPluginInit)
{
    *pnSizeOf_TDataFromAeroSimRC = sizeof(TDataFromAeroSimRC );
    *pnSizeOf_TDataToAeroSimRC   = sizeof(TDataToAeroSimRC   );
    *pnSizeOf_TPluginInit        = sizeof(TPluginInit        );
}

/******************************************************************************
 * This function is called once, just after the dll is loaded
 * - to initialise the top bar menu with your custom menu items
 * - to initialise any other resources (e.g. character map for OSD)
 ******************************************************************************/
SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(TPluginInit *ptPluginInit)
{
    // Save paths for later use
    strcpy(g_strPluginFolder, ptPluginInit->strPluginFolder);
    strcpy(g_strOutputFolder, ptPluginInit->strOutputFolder);

    // Qt code begin
    {
//        dbglog.setParent(this);
        dbglog.setFileName(QString(g_strOutputFolder) + "/dbglog.txt");
        if (!dbglog.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        blueLedTimer.start();

        outSocket = new QUdpSocket();
        QSettings settings(QString(g_strPluginFolder) + "/cc_plugin.ini", QSettings::IniFormat);
        outHost = settings.value("remote_host").toString();
        outPort = settings.value("remote_port").toInt();

        inSocket = new QUdpSocket();
        QString inHost = settings.value("local_host").toString();
        quint16 inPort = settings.value("local_port").toInt();
        if(!inSocket->bind(QHostAddress(inHost), inPort)) {
            QTextStream out(&dbglog);
            out << "local_host: " << inHost
                << " local_port: " << QString::number(inPort)
                << " socket error: " << inSocket->errorString() << "\n";
        }
//        Q_OBJECT_FAKE
//        connect(inSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()), Qt::DirectConnection);

    }
    // Qt code end
}


//-----------------------------------------------------------------------------
void readDatagram()
{
    while (inSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(inSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        quint64 datagramSize;
        datagramSize = inSocket->readDatagram(datagram.data(), datagram.size(),
                                               &sender, &senderPort);

//        processDatagram(datagram);
        sprintf(g_strDebugInfo + strlen(g_strDebugInfo), "\ndatagram size: %d", int(datagramSize));
    }
}

void processDatagram(const QByteArray &data)
{
    sprintf(g_strDebugInfo + strlen(g_strDebugInfo), "\ndatagram size: %d", data.size());
}


void Run_Command_Reset(const TDataFromAeroSimRC *ptDataFromAeroSimRC,
                       TDataToAeroSimRC   *ptDataToAeroSimRC)
{
    // Uncheck menu CheckBoxes
    ptDataToAeroSimRC->Menu_nFlags_MenuItem_New_CheckBox_Status &= (0xFFFFFFFF ^ MASK_MENU_ITEM__ENABLE);

    ptDataToAeroSimRC->Model_nOverrideFlags = 0;

    // Set Model Initial Position
    ptDataToAeroSimRC->Model_nOverrideFlags |= TDataToAeroSimRC::OVR_POS;
    ptDataToAeroSimRC->Model_fNewPosX = ptDataFromAeroSimRC->Scenario_fInitialModelPosX;
    ptDataToAeroSimRC->Model_fNewPosY = ptDataFromAeroSimRC->Scenario_fInitialModelPosY;
    ptDataToAeroSimRC->Model_fNewPosZ = ptDataFromAeroSimRC->Scenario_fInitialModelPosZ;

    // Set Model Initial Attitude
    ptDataToAeroSimRC->Model_nOverrideFlags |= TDataToAeroSimRC::OVR_HPR;
    ptDataToAeroSimRC->Model_fNewHeading = ptDataFromAeroSimRC->Scenario_fInitialModelHeading;
    ptDataToAeroSimRC->Model_fNewPitch   = ptDataFromAeroSimRC->Scenario_fInitialModelPitch;
    ptDataToAeroSimRC->Model_fNewRoll    = ptDataFromAeroSimRC->Scenario_fInitialModelRoll;

    // Stop Vel
    ptDataToAeroSimRC->Model_nOverrideFlags |= TDataToAeroSimRC::OVR_VEL;
    ptDataToAeroSimRC->Model_fNewVelX = 0.0f;
    ptDataToAeroSimRC->Model_fNewVelY = 0.0f;
    ptDataToAeroSimRC->Model_fNewVelZ = 0.0f;

    // Stop Ang Vel
    ptDataToAeroSimRC->Model_nOverrideFlags |= TDataToAeroSimRC::OVR_ANG_VEL;
    ptDataToAeroSimRC->Model_fNewAngVelX = 0.0f;
    ptDataToAeroSimRC->Model_fNewAngVelY = 0.0f;
    ptDataToAeroSimRC->Model_fNewAngVelZ = 0.0f;

    // Print some debug info, although it will only be seen during one frame
    sprintf(g_strDebugInfo + strlen(g_strDebugInfo), "\nRESET");

}

void Run_BlinkLEDs(const TDataFromAeroSimRC *ptDataFromAeroSimRC,
                   TDataToAeroSimRC   *ptDataToAeroSimRC,
                   bool &isEnable)
{
    Q_UNUSED(ptDataFromAeroSimRC);

    if (isEnable) {
        if(blueLedTimer.elapsed() >= blueLedTimeout) {
            ptDataToAeroSimRC->Menu_nFlags_MenuItem_New_CheckBox_Status ^= MASK_MENU_ITEM__LED_BLUE;
            blueLedTimer.restart();
        }
        ptDataToAeroSimRC->Menu_nFlags_MenuItem_New_CheckBox_Status |= MASK_MENU_ITEM__LED_GREEN;
    } else {
        ptDataToAeroSimRC->Menu_nFlags_MenuItem_New_CheckBox_Status &= !MASK_MENU_ITEM__LED_GREEN;
    }
}

/******************************************************************************
 * The string returned in pucOnScreenDebugInfoText will be displayed in AeroSIMRC
 * In this example, the all data received from AeroSIMRC is displayed on the screen
 ******************************************************************************/
void InfoText(const TDataFromAeroSimRC *ptDataFromAeroSimRC, TDataToAeroSimRC *ptDataToAeroSimRC)
{
    sprintf(g_strDebugInfo,
            "----------------------------------------------------------------------\n"
            "Plugin Folder = %s\n"
            "Output Folder = %s\n"
            "nStructSize = %d\n"
            "\n" // Simulation Data
            "fIntegrationTimeStep = %f\n"
            "\n" // Model data
            "fPosX,Y,Z    = (% 8.2f, % 8.2f, % 8.2f)\n"
            "fVelX,Y,Z    = (% 8.2f, % 8.2f, % 8.2f)\n"
            "fAngVelX,Y,Z = (% 8.5f, % 8.5f, % 8.5f)\n"
            "fAccelX,Y,Z  = (% 8.5f, % 8.5f, % 8.5f)\n"
            "\n"
            "Lat, Long   = % 10.5f, % 10.5f\n"
            "\n"
            "fHeightAboveTerrain = %f\n"
            "\n"
            "fHeading = % 6.4f   fPitch = % 6.4f   fRoll = % 6.4f\n"
            "\n"
            "TX Aileron   = % 4.2f    RX Aileron   = % 4.2f\n"
            "TX Elevator  = % 4.2f    RX Elevator  = % 4.2f\n"
            "TX Throttle  = % 4.2f    RX Throttle  = % 4.2f\n"
            "TX Rudder    = % 4.2f    RX Rudder    = % 4.2f\n"
            "TX Channel5  = % 4.2f    RX Channel5  = % 4.2f\n"
            "TX Channel6  = % 4.2f    RX Channel6  = % 4.2f\n"
            "TX Channel7  = % 4.2f    RX Channel7  = % 4.2f\n"
            "TX PluginCh1 = % 4.2f    RX PluginCh1 = % 4.2f\n"
            "TX PluginCh2 = % 4.2f    RX PluginCh2 = % 4.2f\n"
            "\n"
            "MenuItems = %lu\n"
            "\n"
            "Model Initial Pos (%f, %f, %f)\n"
            "Model Initial HPR (%f, %f, %f)\n"
            "\n"
            "WP Home X,Y = (% 8.2f, % 8.2f)  Lat, Long = (% 10.5f, % 10.5f)  Description %s\n"
            "WP A    X,Y = (% 8.2f, % 8.2f)  Lat, Long = (% 10.5f, % 10.5f)  Description %s\n"
            "WP B    X,Y = (% 8.2f, % 8.2f)  Lat, Long = (% 10.5f, % 10.5f)  Description %s\n"
            "WP C    X,Y = (% 8.2f, % 8.2f)  Lat, Long = (% 10.5f, % 10.5f)  Description %s\n"
            "WP D    X,Y = (% 8.2f, % 8.2f)  Lat, Long = (% 10.5f, % 10.5f)  Description %s\n"
            "\n"
            "Engine1 = % 5.0f RPM   Engine2 = % 5.0f RPM   Engine3 = % 5.0f RPM   Engine4 = % 5.0f RPM\n"
            "\n"
            "Wind = (% 8.2f, % 8.2f, % 8.2f)\n"
            "\n"
            "Battery % 2.1f V  % 2.1f A   Consumed = % 2.3f Ah   Capacity = % 2.3f Ah\n"
            "Fuel Consumed = % 2.3f l    Tank Capacity = % 2.3f l\n"
            "----------------------------------------------------------------------\n"
            ,
            g_strPluginFolder,
            g_strOutputFolder,
            ptDataFromAeroSimRC->nStructSize,

            // Simulation Data
            ptDataFromAeroSimRC->Simulation_fIntegrationTimeStep,

            // Model data
            ptDataFromAeroSimRC->Model_fPosX,     ptDataFromAeroSimRC->Model_fPosY,     ptDataFromAeroSimRC->Model_fPosZ,
            ptDataFromAeroSimRC->Model_fVelX,     ptDataFromAeroSimRC->Model_fVelY,     ptDataFromAeroSimRC->Model_fVelZ,
            ptDataFromAeroSimRC->Model_fAngVelX,  ptDataFromAeroSimRC->Model_fAngVelY,  ptDataFromAeroSimRC->Model_fAngVelZ,
            ptDataFromAeroSimRC->Model_fAccelX,   ptDataFromAeroSimRC->Model_fAccelY,   ptDataFromAeroSimRC->Model_fAccelZ,

            ptDataFromAeroSimRC->Model_fLatitude, ptDataFromAeroSimRC->Model_fLongitude,

            ptDataFromAeroSimRC->Model_fHeightAboveTerrain,

            ptDataFromAeroSimRC->Model_fHeading, ptDataFromAeroSimRC->Model_fPitch, ptDataFromAeroSimRC->Model_fRoll,

            ptDataFromAeroSimRC->Channel_afValue_TX[CH_AILERON ],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_AILERON ],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_ELEVATOR],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_ELEVATOR],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_THROTTLE],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_THROTTLE],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_RUDDER  ],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_RUDDER  ],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_5       ],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_5       ],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_6       ],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_6       ],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_7       ],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_7       ],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_PLUGIN_1],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_PLUGIN_1],
            ptDataFromAeroSimRC->Channel_afValue_TX[CH_PLUGIN_2],  ptDataFromAeroSimRC->Channel_afValue_RX[CH_PLUGIN_2],

            ptDataFromAeroSimRC->Menu_nFlags_MenuItem_Status,

            ptDataFromAeroSimRC->Scenario_fInitialModelPosX,    ptDataFromAeroSimRC->Scenario_fInitialModelPosY,  ptDataFromAeroSimRC->Scenario_fInitialModelPosZ,
            ptDataFromAeroSimRC->Scenario_fInitialModelHeading, ptDataFromAeroSimRC->Scenario_fInitialModelPitch, ptDataFromAeroSimRC->Scenario_fInitialModelRoll,

            ptDataFromAeroSimRC->Scenario_fWPHome_X, ptDataFromAeroSimRC->Scenario_fWPHome_Y,  ptDataFromAeroSimRC->Scenario_fWPHome_Lat, ptDataFromAeroSimRC->Scenario_fWPHome_Long, ptDataFromAeroSimRC->Scenario_strWPHome_Description,
            ptDataFromAeroSimRC->Scenario_fWPA_X,    ptDataFromAeroSimRC->Scenario_fWPA_Y,     ptDataFromAeroSimRC->Scenario_fWPA_Lat,    ptDataFromAeroSimRC->Scenario_fWPA_Long,    ptDataFromAeroSimRC->Scenario_strWPA_Description,
            ptDataFromAeroSimRC->Scenario_fWPB_X,    ptDataFromAeroSimRC->Scenario_fWPB_Y,     ptDataFromAeroSimRC->Scenario_fWPB_Lat,    ptDataFromAeroSimRC->Scenario_fWPB_Long,    ptDataFromAeroSimRC->Scenario_strWPB_Description,
            ptDataFromAeroSimRC->Scenario_fWPC_X,    ptDataFromAeroSimRC->Scenario_fWPC_Y,     ptDataFromAeroSimRC->Scenario_fWPC_Lat,    ptDataFromAeroSimRC->Scenario_fWPC_Long,    ptDataFromAeroSimRC->Scenario_strWPC_Description,
            ptDataFromAeroSimRC->Scenario_fWPD_X,    ptDataFromAeroSimRC->Scenario_fWPD_Y,     ptDataFromAeroSimRC->Scenario_fWPD_Lat,    ptDataFromAeroSimRC->Scenario_fWPD_Long,    ptDataFromAeroSimRC->Scenario_strWPD_Description,

            ptDataFromAeroSimRC->Model_fEngine1_RPM, ptDataFromAeroSimRC->Model_fEngine2_RPM, ptDataFromAeroSimRC->Model_fEngine3_RPM, ptDataFromAeroSimRC->Model_fEngine4_RPM,

            ptDataFromAeroSimRC->Model_fWindVelX, ptDataFromAeroSimRC->Model_fWindVelY, ptDataFromAeroSimRC->Model_fWindVelZ,

            ptDataFromAeroSimRC->Model_fBatteryVoltage, ptDataFromAeroSimRC->Model_fBatteryCurrent, ptDataFromAeroSimRC->Model_fBatteryConsumedCharge, ptDataFromAeroSimRC->Model_fBatteryCapacity,

            ptDataFromAeroSimRC->Model_fFuelConsumed, ptDataFromAeroSimRC->Model_fFuelTankCapacity
            );

}


/******************************************************************************
 * This function is called at each program cycle from AeroSIM RC
 ******************************************************************************/
SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(const TDataFromAeroSimRC *ptDataFromAeroSimRC,
                                         TDataToAeroSimRC   *ptDataToAeroSimRC)
{
    // debug info is shown on the screen
    ptDataToAeroSimRC->Debug_pucOnScreenInfoText = g_strDebugInfo;

    InfoText(ptDataFromAeroSimRC, ptDataToAeroSimRC);

    // By default do not change the Menu Items of type CheckBox
    ptDataToAeroSimRC->Menu_nFlags_MenuItem_New_CheckBox_Status = ptDataFromAeroSimRC->Menu_nFlags_MenuItem_Status;

    // Extract Menu Commands from Flags
    bool bCommand_Reset = (ptDataFromAeroSimRC->Menu_nFlags_MenuItem_Status & MASK_MENU_ITEM__COMMAND_RESET) != 0;
    bool isEnable = (ptDataFromAeroSimRC->Menu_nFlags_MenuItem_Status & MASK_MENU_ITEM__ENABLE) != 0;
    bool isTxEnable = (ptDataFromAeroSimRC->Menu_nFlags_MenuItem_Status & MASK_MENU_ITEM__TX) != 0;
    bool isRxEnable = (ptDataFromAeroSimRC->Menu_nFlags_MenuItem_Status & MASK_MENU_ITEM__RX) != 0;

    // Run commands
    if(bCommand_Reset) {
        Run_Command_Reset(ptDataFromAeroSimRC, ptDataToAeroSimRC);
    } else {
        Run_BlinkLEDs(ptDataFromAeroSimRC, ptDataToAeroSimRC, isEnable);

        // network
        if (isEnable) {
            // send data
            if (isTxEnable) {
                QByteArray data;
                // 220 - current size of datagram, change it if recieved data is changed!!!
                data.resize(220);
                QDataStream stream(&data, QIODevice::WriteOnly);
                // magic header, "AERO"
                stream << quint32(0x4153494D);
                // home location
                stream << qreal(ptDataFromAeroSimRC->Scenario_fInitialModelPosX);
                stream << qreal(ptDataFromAeroSimRC->Scenario_fInitialModelPosY);
                stream << qreal(ptDataFromAeroSimRC->Scenario_fInitialModelPosZ);
                stream << qreal(ptDataFromAeroSimRC->Scenario_fWPHome_X);
                stream << qreal(ptDataFromAeroSimRC->Scenario_fWPHome_Y);
                stream << qreal(ptDataFromAeroSimRC->Scenario_fWPHome_Lat);
                stream << qreal(ptDataFromAeroSimRC->Scenario_fWPHome_Long);
                // position actual
                stream << qreal(ptDataFromAeroSimRC->Model_fPosX);
                stream << qreal(ptDataFromAeroSimRC->Model_fPosY);
                stream << qreal(ptDataFromAeroSimRC->Model_fPosZ);
                // velocity
                stream << qreal(ptDataFromAeroSimRC->Model_fVelX);
                stream << qreal(ptDataFromAeroSimRC->Model_fVelY);
                stream << qreal(ptDataFromAeroSimRC->Model_fVelZ);
                // angular velocity
                stream << qreal(ptDataFromAeroSimRC->Model_fAngVelX);
                stream << qreal(ptDataFromAeroSimRC->Model_fAngVelY);
                stream << qreal(ptDataFromAeroSimRC->Model_fAngVelZ);
                // acceleration
                stream << qreal(ptDataFromAeroSimRC->Model_fAccelX);
                stream << qreal(ptDataFromAeroSimRC->Model_fAccelY);
                stream << qreal(ptDataFromAeroSimRC->Model_fAccelZ);
                // coordinates
                stream << qreal(ptDataFromAeroSimRC->Model_fLatitude);
                stream << qreal(ptDataFromAeroSimRC->Model_fLongitude);
                // sonar :)
                stream << qreal(ptDataFromAeroSimRC->Model_fHeightAboveTerrain);
                // attitude
                stream << qreal(ptDataFromAeroSimRC->Model_fHeading);
                stream << qreal(ptDataFromAeroSimRC->Model_fPitch);
                stream << qreal(ptDataFromAeroSimRC->Model_fRoll);
                // electric
                stream << qreal(ptDataFromAeroSimRC->Model_fBatteryVoltage);
                stream << qreal(ptDataFromAeroSimRC->Model_fBatteryCurrent);

                // send data
                if(outSocket->writeDatagram(data, outHost, outPort) == -1) {
                    QTextStream out(&dbglog);
                    out << "host: " << outHost.toString()
                        << " port: " << QString::number(outPort)
                        << " socket error: " << outSocket->errorString() << "\n";
                }
            }
            // recive data
            if(isRxEnable) {
                if(inSocket->hasPendingDatagrams())
                    readDatagram();
            }
        }
    }

    g_bFirstRun = false;
}

/******************************************************************************
 * This function is called at close plugin (in 3.81 it does not exist)
 ******************************************************************************/
SIM_DLL_EXPORT void AeroSIMRC_Plugin_Close()
{
    if(outSocket) {
        delete outSocket;
        outSocket = NULL;
    }
    if(inSocket) {
        delete inSocket;
        inSocket = NULL;
    }
    // delete other used resources
}
