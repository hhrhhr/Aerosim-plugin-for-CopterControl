#ifndef DATATOAEROSIM_H
#define DATATOAEROSIM_H

#ifndef AEROSIMRC_MAX_CHANNELS
#define AEROSIMRC_MAX_CHANNELS 39
#endif

enum EOverrideFlags
{
    OVR_POS             = (1 << 0),
    OVR_VEL             = (1 << 1),
    OVR_ANG_VEL         = (1 << 2),
    OVR_HPR             = (1 << 3),  // Override Heading, Pitch and Roll
    OVR_WIND_VEL        = (1 << 4),  // Override Wind velocity at model
    OVR_ENGINE_RPM      = (1 << 5),  // Override RPM of all Engines or Motors
    OVR_BAT_VOLT        = (1 << 6),  // Override motor Battery Voltage
    OVR_BAT_AMP         = (1 << 7),  // Override motor Battery current
    OVR_BAT_AH_CONSUMED = (1 << 8),  // Override motor Battery AmpsHour consumed
    OVR_FUEL_CONSUMED   = (1 << 9)   // Override Fuel consumed (gas & jet engines)
};

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
}__attribute__((packed));

// normal 516
// packed 507
// packed and enum out - 507

#endif // DATATOAEROSIM_H
