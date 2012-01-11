#ifndef _plugin_h_
#define _plugin_h_

#include <QtCore>
/*// Qt includes begin
#include <QByteArray>
#include <QTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDebug>
// Qt includes end
*/

// Make sure the struct is byte aligned
// Custom Menu Item masks
enum MenuMasks {
    MenuCmdReset    = 1 << 0,
    MenuEnable      = 1 << 1,
    MenuTx          = 1 << 2,
    MenuRx          = 1 << 3,
    MenuLedBlue     = 1 << 4,
    MenuLedGreen    = 1 << 5
};

#pragma pack (push, r1, 1)

#define AEROSIMRC_MAX_CHANNELS 39
#include "datafromaerosim.h"
#include "datatoaerosim.h"
#include "plugininit.h"

#define SIM_DLL_EXPORT extern "C" __declspec(dllexport)

SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(
        quint32 *sizeSimToPlugin,
        quint32 *sizePluginToSim,
        quint32 *sizePluginInit
        );

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(
        pluginInit *p
        );

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(
        const simToPlugin *stp,
              pluginToSim *pts
        );

#pragma pack (pop, r1)

#endif // _plugin_h_
