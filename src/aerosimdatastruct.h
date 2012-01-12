#ifndef DATAFROMAEROSIM_H
#define DATAFROMAEROSIM_H

#include <QtCore>

#define AEROSIMRC_MAX_CHANNELS 39
#define DBG_BUFFER_MAX_SIZE 4096

#define MAX_DLL_USER_MENU_ITEMS 16
#define OBSOLETE_MIT_COMMAND    (1 << 0)
#define OBSOLETE_MIT_CHECKBOX   (1 << 1)
#define OBSOLETE_MIT_SEPARATOR  (1 << 7)

#if defined(Q_CC_MSVC)
#define PACK_STRUCT
#define MAX_PATH 260
#pragma pack (push, r1, 1)
#elif defined(Q_CC_GNU)
#define PACK_STRUCT __attribute__((packed))
#endif

struct simToPlugin
{
    unsigned short structSize;
    float simTimeStep;
    float chSimTX[AEROSIMRC_MAX_CHANNELS];
    float chSimRX[AEROSIMRC_MAX_CHANNELS];
    unsigned char *OSDVideoBuf;
    unsigned long simMenuStatus;
    float initPosX;
    float initPosY;
    float initPosZ;
    float initHeading;
    float initPitch;
    float initRoll;
    float wpHomeX;
    float wpHomeY;
    float wpHomeLat;
    float wpHomeLong;
    const char *wpHomeDesc; // (m, deg, string)
    float wpAX;
    float wpAY;
    float wpALat;
    float wpALong;
    const char *wpADesc;    // (m, deg, string)
    float wpBX;
    float wpBY;
    float wpBLat;
    float wpBLong;
    const char *wpBDesc;    // (m, deg, string)
    float wpCX;
    float wpCY;
    float wpCLat;
    float wpCLong;
    const char *wpCDesc;    // (m, deg, string)
    float wpDX;
    float wpDY;
    float wpDLat;
    float wpDLong;
    const char *wpDDesc;    // (m, deg, string)
    float posX;
    float posY;
    float posZ;
    float velX;
    float velY;
    float velZ;
    float angVelX;
    float angVelY;
    float angVelZ;
    float accelX;
    float accelY;
    float accelZ;
    double latitude;
    double longitude;
    float AGL;
    float heading;
    float pitch;
    float roll;
    float windVelX;
    float windVelY;
    float windVelZ;
    float eng1RPM;
    float eng2RPM;
    float eng3RPM;
    float eng4RPM;
    float voltage;          // V
    float current;          // A
    float consumedCharge;   // Ah
    float capacity;         // Ah
    float fuelConsumed;            // l
    float fuelTankCapacity;        // l
} PACK_STRUCT ;     // normal - 592 packed - 582 OK (3.81)

struct pluginToSim
{
    unsigned short structSize;
    const char *dbgInfoText;
    unsigned char chOverTX[AEROSIMRC_MAX_CHANNELS];
    float chNewTX[AEROSIMRC_MAX_CHANNELS];
    unsigned char chOverRX[AEROSIMRC_MAX_CHANNELS];
    float chNewRX[AEROSIMRC_MAX_CHANNELS];
    float newPosX;
    float newPosY;
    float newPosZ;
    float newVelX;
    float newVelY;
    float newVelZ;
    float newAngVelX;
    float newAngVelY;
    float newAngVelZ;
    float newHeading;
    float newPitch;
    float newRoll;
    unsigned long modelOverrideFlags;
    unsigned long newMenuStatus;
    unsigned char isOSDShow;
    unsigned char isOSDChanged;
    unsigned short OSDWindow_DX;
    unsigned short OSDWindow_DY;
    float OSDScale;
    float newWindVelX;
    float newWindVelY;
    float newWindVelZ;
    float newEng1RPM;
    float newEng2RPM;
    float newEng3RPM;
    float newEng4RPM;
    float newVoltage;
    float newCurrent;
    float newConsumedCharge;
    float newFuelConsumed;
    unsigned char modelCrashInhibit;
} PACK_STRUCT ;     // normal 516, packed 507 OK (3.81)

struct TPluginMenuItem
{
    unsigned long OBSOLETE_eType;
    char *OBSOLETE_strName;
} PACK_STRUCT ;

struct pluginInit
{
    unsigned long nStructSize;
    char *OBSOLETE_strMenuTitle;
    TPluginMenuItem OBSOLETE_atMenuItem[MAX_DLL_USER_MENU_ITEMS];
    const char *strPluginFolder;
    const char *strOutputFolder;
} PACK_STRUCT ;     // normal - 144, packed - 144 OK (3.81)

#ifdef Q_CC_MSVC
#pragma pack (pop, r1)
#endif
#undef PACK_STRUCT

#endif // DATAFROMAEROSIM_H
