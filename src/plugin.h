#ifndef _plugin_AeroSIMRC_h_
#define _plugin_AeroSIMRC_h_

// Qt includes begin
#include <QtCore>
#include <QTime>
#include <QUdpSocket>
#include <QSettings>
#include <QFile>
#include <QTextStream>
// Qt includes end

// Make sure the struct is byte aligned
#pragma pack (push, r1, 1)

#define AEROSIMRC_MAX_CHANNELS 39

#include "datafromaerosim.h"
#include "datatoaerosim.h"
#include "plugininit.h"

#define SIM_DLL_EXPORT extern "C" __declspec(dllexport)

/******************************************************************************
 * Handling Compatibility with future AeroSIM RC versions
 * In newer versions of AeroSIM RC, the structs TDataFromAeroSimRC and TDataToAeroSimRC
 * may be extended with new data.
 * The new data will always be appended at the end of the previous struct version.
 * This insures backward compatibility for plugins created with older versions of AeroSIM RC
 * AeroSIM RC will prevent loading a plugin which struct sizes are larger than the struct sizes
 * of the installed AeroSIM RC.
 ******************************************************************************/
SIM_DLL_EXPORT void AeroSIMRC_Plugin_ReportStructSizes(
        unsigned long *pnSizeOf_TDataFromAeroSimRC,
        unsigned long *pnSizeOf_TDataToAeroSimRC,
        unsigned long *pnSizeOf_TPluginInit
        );

/******************************************************************************
 * Used to optionally define menu items in the top menu bar.
 * Also used to initialise the plugin (e.g. loading data files)
 ******************************************************************************/
SIM_DLL_EXPORT void AeroSIMRC_Plugin_Init(
        TPluginInit *pTPluginInit
        );

/******************************************************************************
 * This is the  *HEART*  of the plugin. It is called every simulation cycle.
 ******************************************************************************/
SIM_DLL_EXPORT void AeroSIMRC_Plugin_Run(
        const TDataFromAeroSimRC *pTDataFromAeroSimRC,
              TDataToAeroSimRC   *ptDataToAeroSimRC
        );


/******************************************************************************
 * (from hhrhhr) I think, what this function would be added later :)
 ******************************************************************************/
SIM_DLL_EXPORT void AeroSIMRC_Plugin_Close();

#pragma pack (pop, r1)

#endif // _plugin_AeroSIMRC_h_
