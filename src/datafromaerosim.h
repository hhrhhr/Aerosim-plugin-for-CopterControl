#ifndef DATAFROMAEROSIM_H
#define DATAFROMAEROSIM_H

enum {
    ch_aileron,
    ch_elevator,
    ch_throttle,
    ch_rudder,
    ch_5,
    ch_6,
    ch_7,
    ch_8,
    ch_9,
    ch_gear,
    ch_flaps,
    ch_fpv_pan,
    ch_fpv_tilt,
    ch_brakes,
    ch_spoiler,
    ch_smoke,
    ch_fire,
    ch_f_mode,
    ch_alt_hold,
    ch_fpv_hold,
    ch_reset,
    ch_mouse_tx,
    ch_plugin_1,
    ch_plugin_2,
    ch_thr_hold,
    ch_carefree,
    ch_27,
    ch_l_motor,
    ch_r_motor,
    ch_30_mix,
    ch_31_mix,
    ch_32_mix,
    ch_33_mix,
    ch_34_mix,
    ch_35_mix,
    ch_36_mix,
    ch_37_mix,
    ch_38_mix,
    ch_39_mix
};

struct simToPlugin
{
    unsigned short nStructSize;
    float Simulation_fIntegrationTimeStep;
    float Channel_afValue_TX[AEROSIMRC_MAX_CHANNELS];
    float Channel_afValue_RX[AEROSIMRC_MAX_CHANNELS];
    unsigned char *OSD_pVideoBuffer;
    unsigned long Menu_nFlags_MenuItem_Status;
    float Scenario_fInitialModelPosX;
    float Scenario_fInitialModelPosY;
    float Scenario_fInitialModelPosZ;
    float Scenario_fInitialModelHeading;
    float Scenario_fInitialModelPitch;
    float Scenario_fInitialModelRoll;
    float Scenario_fWPHome_X;
    float Scenario_fWPHome_Y;
    float Scenario_fWPHome_Lat;
    float Scenario_fWPHome_Long;
    const char *Scenario_strWPHome_Description; // (m, deg, string)
    float Scenario_fWPA_X;
    float Scenario_fWPA_Y;
    float Scenario_fWPA_Lat;
    float Scenario_fWPA_Long;
    const char *Scenario_strWPA_Description;    // (m, deg, string)
    float Scenario_fWPB_X;
    float Scenario_fWPB_Y;
    float Scenario_fWPB_Lat;
    float Scenario_fWPB_Long;
    const char *Scenario_strWPB_Description;    // (m, deg, string)
    float Scenario_fWPC_X;
    float Scenario_fWPC_Y;
    float Scenario_fWPC_Lat;
    float Scenario_fWPC_Long;
    const char *Scenario_strWPC_Description;    // (m, deg, string)
    float Scenario_fWPD_X;
    float Scenario_fWPD_Y;
    float Scenario_fWPD_Lat;
    float Scenario_fWPD_Long;
    const char *Scenario_strWPD_Description;    // (m, deg, string)
    float Model_fPosX;
    float Model_fPosY;
    float Model_fPosZ;
    float Model_fVelX;
    float Model_fVelY;
    float Model_fVelZ;
    float Model_fAngVelX;
    float Model_fAngVelY;
    float Model_fAngVelZ;
    float Model_fAccelX;
    float Model_fAccelY;
    float Model_fAccelZ;
    double Model_fLatitude;
    double Model_fLongitude;
    float  Model_fHeightAboveTerrain;
    float Model_fHeading;
    float Model_fPitch;
    float Model_fRoll;
    float Model_fWindVelX;
    float Model_fWindVelY;
    float Model_fWindVelZ;
    float  Model_fEngine1_RPM;
    float  Model_fEngine2_RPM;
    float  Model_fEngine3_RPM;
    float  Model_fEngine4_RPM;
    float  Model_fBatteryVoltage;          // V
    float  Model_fBatteryCurrent;          // A
    float  Model_fBatteryConsumedCharge;   // Ah
    float  Model_fBatteryCapacity;         // Ah
    float  Model_fFuelConsumed;            // l
    float  Model_fFuelTankCapacity;        // l
};

#endif // DATAFROMAEROSIM_H
