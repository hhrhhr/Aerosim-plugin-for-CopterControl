#ifndef _plugin_h_
#define _plugin_h_

/*// Qt includes begin
#include <QtCore>
#include <QTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDebug>
// Qt includes end
*/

// Make sure the struct is byte aligned
#pragma pack (push, r1, 1)

#define AEROSIMRC_MAX_CHANNELS 39

#include "datafromaerosim.h"
#include "datatoaerosim.h"
#include "plugininit.h"

#define SIM_DLL_EXPORT extern "C" __declspec(dllexport)

SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(
        unsigned long *pnSizeOf_TDataFromAeroSimRC,
        unsigned long *pnSizeOf_TDataToAeroSimRC,
        unsigned long *pnSizeOf_TPluginInit
        );

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(
        pluginInit *pTPluginInit
        );

SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(
        const simToPlugin *pTDataFromAeroSimRC,
              pluginToSim   *ptDataToAeroSimRC
        );

#pragma pack (pop, r1)

#endif // _plugin_h_
