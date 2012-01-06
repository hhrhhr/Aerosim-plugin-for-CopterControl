#ifndef DATAFROMAEROSIM_H
#define DATAFROMAEROSIM_H

/*
 * Two main data structures are used. This is the first one:
 * This data struct is filled by AeroSIM RC with the simulation data, and sent to the plugin
 *
 */

/*
 * Use the following constants as indexes for the channel arrays
 * The simulator uses internally the channel numbers for Transmitter Mode 2
 * (regardless of mode selected by user)
 */
#define CH_AILERON      0
#define CH_ELEVATOR     1
#define CH_THROTTLE     2
#define CH_RUDDER       3
#define CH_5            4
#define CH_6            5
#define CH_7            6

// This channel is mapped by user to any real channel number
#define CH_PLUGIN_1     22
#define CH_PLUGIN_2     23

struct TDataFromAeroSimRC
{

    // size in bytes of TDataFromAeroSimRC
    unsigned short nStructSize;

    // integration time step in seconds.
    // This is the simulated time since last call to AeroSIMRC_Plugin_Run()
    float Simulation_fIntegrationTimeStep;

    // [-1, 1] channel positions at TX sticks (i.e. raw stick positions)
    float Channel_afValue_TX[AEROSIMRC_MAX_CHANNELS];

    // [-1, 1] channel positions at RX (i.e. after TX mixes)
    float Channel_afValue_RX[AEROSIMRC_MAX_CHANNELS];

/*
 * Video buffer for OSD is a bitmap, 4 bytes per pixel: R G B A;
 * The first 4 bytes are the Top-Left corner pixel
 * The size of the OSD Video Buffer is defined in plugin.txt (.OSD_BUFFER_SIZE),
 * can be set to one of the following sizes: 512x512, 1024x512 or 1024x1024
 * Set OSD_nWindow_DX and OSD_nWindow_DY in struct TDataToAeroSimRC to the actual size to be displayed
 *
 */
    unsigned char *OSD_pVideoBuffer;

/*
 * This variable represent the custom menu status. E.g. 0x000001 means that first menu item is ticked
 * Command  menu item bits are set to 1 when selected, but cleared in the next cycle.
 * Checkbox menu item bits remain 1 until unchecked by user,
 * or cleared in TDataToAeroSimRC::Menu_nFlags_MenuItem_New_CheckBox_Status
 *
 */
    unsigned long Menu_nFlags_MenuItem_Status;

    // Model Initial Position in current scenario
    // (m) Model Initial Position on runway
    float Scenario_fInitialModelPosX;
    float Scenario_fInitialModelPosY;
    float Scenario_fInitialModelPosZ;

    // (m) Model Initial Attitude on runway
    float Scenario_fInitialModelHeading;
    float Scenario_fInitialModelPitch;
    float Scenario_fInitialModelRoll;

    //---------------------
    // WayPoints
    // The Description string can be freely used to add more information to the waypoint
    // such as Altitude, WP Type (Overfly, Landing, CAP), Bearing, etc.
    //---------------------
    float Scenario_fWPHome_X;   float Scenario_fWPHome_Y;
    float Scenario_fWPHome_Lat; float Scenario_fWPHome_Long;
    const char *Scenario_strWPHome_Description; // (m, deg, string)

    float Scenario_fWPA_X;      float Scenario_fWPA_Y;
    float Scenario_fWPA_Lat;    float Scenario_fWPA_Long;
    const char *Scenario_strWPA_Description;    // (m, deg, string)

    float Scenario_fWPB_X;      float Scenario_fWPB_Y;
    float Scenario_fWPB_Lat;    float Scenario_fWPB_Long;
    const char *Scenario_strWPB_Description;    // (m, deg, string)

    float Scenario_fWPC_X;      float Scenario_fWPC_Y;
    float Scenario_fWPC_Lat;    float Scenario_fWPC_Long;
    const char *Scenario_strWPC_Description;    // (m, deg, string)

    float Scenario_fWPD_X;      float Scenario_fWPD_Y;
    float Scenario_fWPD_Lat;    float Scenario_fWPD_Long;
    const char *Scenario_strWPD_Description;    // (m, deg, string)

    // m      Model absolute position in scenario (X=Right, Y=Front, Z=Up)
    float Model_fPosX;
    float Model_fPosY;
    float Model_fPosZ;

    // m/s    Model velocity
    float Model_fVelX;
    float Model_fVelY;
    float Model_fVelZ;

    // rad/s  Model angular velocity (useful to implement gyroscopes)
    float Model_fAngVelX;
    float Model_fAngVelY;
    float Model_fAngVelZ;

    // m/s/s  Model acceleration (useful to implement accelerometers)
    float Model_fAccelX;
    float Model_fAccelY;
    float Model_fAccelZ;

    // deg    Model Position in Lat/Long coordinates
    double Model_fLatitude;
    double Model_fLongitude;

    // m
    float  Model_fHeightAboveTerrain;

    // rad [-PI,   PI  ] 0 = North, PI/2 = East, PI = South, - PI/2 = West
    float Model_fHeading;
    // rad [-PI/2, PI/2] Positive pitch when nose up
    float Model_fPitch;
    // rad [-PI,   PI  ] Positive roll when right wing Up
    float Model_fRoll;

    // m/s   Velocity of the wind (with gusts) at model position (useful to compute air vel)
    float Model_fWindVelX;
    float Model_fWindVelY;
    float Model_fWindVelZ;

    // Engine/Motor Revs per minute
    float  Model_fEngine1_RPM;
    float  Model_fEngine2_RPM;
    float  Model_fEngine3_RPM;
    float  Model_fEngine4_RPM;

    // Battery (electric models)
    float  Model_fBatteryVoltage;          // V
    float  Model_fBatteryCurrent;          // A
    float  Model_fBatteryConsumedCharge;   // Ah
    float  Model_fBatteryCapacity;         // Ah

    // Fuel (gas & jet models)
    float  Model_fFuelConsumed;            // l
    float  Model_fFuelTankCapacity;        // l

};

#endif // DATAFROMAEROSIM_H
