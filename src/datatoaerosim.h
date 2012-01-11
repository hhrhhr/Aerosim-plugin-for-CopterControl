#ifndef DATATOAEROSIM_H
#define DATATOAEROSIM_H

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
    unsigned short nStructSize;
    const char *Debug_pucOnScreenInfoText;
    unsigned char Channel_abOverride_TX[AEROSIMRC_MAX_CHANNELS];
    float Channel_afNewValue_TX[AEROSIMRC_MAX_CHANNELS];
    unsigned char Channel_abOverride_RX[AEROSIMRC_MAX_CHANNELS];
    float Channel_afNewValue_RX[AEROSIMRC_MAX_CHANNELS];
    float Model_fNewPosX;
    float Model_fNewPosY;
    float Model_fNewPosZ;
    float Model_fNewVelX;
    float Model_fNewVelY;
    float Model_fNewVelZ;
    float Model_fNewAngVelX;
    float Model_fNewAngVelY;
    float Model_fNewAngVelZ;
    float Model_fNewHeading;
    float Model_fNewPitch;
    float Model_fNewRoll;
    unsigned long Model_nOverrideFlags;
    unsigned long Menu_nFlags_MenuItem_New_CheckBox_Status;
    unsigned char OSD_bShow;
    unsigned char OSD_bHasChanged;
    unsigned short OSD_nWindow_DX;
    unsigned short OSD_nWindow_DY;
    float OSD_fScale;
    float Model_fNewWindVelX;
    float Model_fNewWindVelY;
    float Model_fNewWindVelZ;
    float Model_fNewEngine1_RPM;
    float Model_fNewEngine2_RPM;
    float Model_fNewEngine3_RPM;
    float Model_fNewEngine4_RPM;
    float  Model_fNewBatteryVoltage;
    float  Model_fNewBatteryCurrent;
    float  Model_fNewBatteryConsumedCharge;
    float  Model_fNewFuelConsumed;
    unsigned char Model_bCrashInhibit;
};

#endif // DATATOAEROSIM_H
