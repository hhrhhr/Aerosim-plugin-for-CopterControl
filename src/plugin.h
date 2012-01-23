#ifndef _plugin_h_
#define _plugin_h_

#include <QtCore>
#include <QTime>
#include "aerosimdatastruct.h"

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

#endif // _plugin_h_
