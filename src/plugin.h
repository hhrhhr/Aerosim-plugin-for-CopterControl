#ifndef _plugin_h_
#define _plugin_h_

#include <QtCore>
#include "qdebughandler.h"
#include "udpconnect.h"

// Custom Menu Item masks
enum MenuMasks {
    MenuCmdReset    = 1 << 0,
    MenuEnable      = 1 << 1,
    MenuTx          = 1 << 2,
    MenuRx          = 1 << 3,
    MenuLedBlue     = 1 << 4,
    MenuLedGreen    = 1 << 5
};

enum Channels {
    ChAileron,
    ChElevator,
    ChThrottle,
    ChRudder,
    Ch5,
    Ch6,
    Ch7,
    Ch8,
    Ch9,
    ChGear,
    ChFlaps,
    ChFpvPan,
    ChFpvTilt,
    ChBrakes,
    ChSpoiler,
    ChSmoke,
    ChFire,
    ChFMode,
    ChAltHold,
    ChFpvHold,
    ChReset,
    ChMouseTx,
    ChPlugin1,
    ChPlugin2,
    ChThrHold,
    ChCareFree,
    Ch27,
    ChLMotor,
    ChRMotor,
    Ch30Mix,
    Ch31Mix,
    Ch32Mix,
    Ch33Mix,
    Ch34Mix,
    Ch35Mix,
    Ch36Mix,
    Ch37Mix,
    Ch38Mix,
    Ch39Mix
};

#include "datafromaerosim.h"
#include "datatoaerosim.h"
#include "plugininit.h"

#define SIM_DLL_EXPORT extern "C" __declspec(dllexport)

SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(
        unsigned long *sizeSimToPlugin,
        unsigned long *sizePluginToSim,
        unsigned long *sizePluginInit
        );

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(
        pluginInit *p
        );

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(
        const simToPlugin *stp,
              pluginToSim *pts
        );

#endif // _plugin_h_
