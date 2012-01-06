#ifndef DATATOAEROSIM_H
#define DATATOAEROSIM_H

/*
 * This is the second main data structure.
 * This data struct is filled by the plugin and returned to AeroSIM RC
 *
 */

struct TDataToAeroSimRC
{
  // Size in bytes of struct TDataToAeroSimRC (this is filled by AeroSIM RC)
  unsigned short nStructSize;

  // The plugin can compose a string that will be displayed in AeroSIM RC for debugging purposes
  // Edit file config2.txt and set I_AM_A_PLUGIN_DEVELOPER to 1
  // to enable the menu option to display this string
  // Once enabled, this option can be found in the plugin submenu at the Menu Top Bar
  const char *Debug_pucOnScreenInfoText;

  // TX channel (i.e. stick positions) before mixes in the simulated TX
  // true to override the value of this channel
  unsigned char Channel_abOverride_TX[AEROSIMRC_MAX_CHANNELS];

  // new value for TX sticks
  float Channel_afNewValue_TX[AEROSIMRC_MAX_CHANNELS];

  // RX channels as received at RX, including simulated TX mixes
  // true to override the value of this channel
  unsigned char Channel_abOverride_RX[AEROSIMRC_MAX_CHANNELS];

  // new value for RX channels
  float Channel_afNewValue_RX[AEROSIMRC_MAX_CHANNELS];

  // Model data to be overridden
  // m    (X = right, Y = front, Z = up)
  float Model_fNewPosX;
  float Model_fNewPosY;
  float Model_fNewPosZ;

  // m/s
  float Model_fNewVelX;
  float Model_fNewVelY;
  float Model_fNewVelZ;

  // rad/s
  float Model_fNewAngVelX;
  float Model_fNewAngVelY;
  float Model_fNewAngVelZ;

  // rad
  float Model_fNewHeading;
  float Model_fNewPitch;
  float Model_fNewRoll;

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

  // Used to specify which data is to be overridden. E.g. Model_nOverrideFlags = OVR_POS | OVR_VEL
  unsigned long Model_nOverrideFlags;

  // Menu Check Box update from plugin
  // Allows to change the status of Menu Items of type CheckBox (see example 'Reset')
  unsigned long Menu_nFlags_MenuItem_New_CheckBox_Status;

  // Set to true to show the OSD.
  // This saves the time required to render an empty OSD when nothing is to be displayed.
  unsigned char OSD_bShow;

  // Set to true if the OSD video buffer was written with new data.
  // This saves the time required to upload the new image to the graphics card.
  unsigned char OSD_bHasChanged;

  // Defines the window size within the VideoBufferOSD to be drawn
  // (from top-left corner of the video buffer)
  unsigned short OSD_nWindow_DX;
  unsigned short OSD_nWindow_DY;

  // 1.0 to expand OSD to the size of the screen (while keeping tha aspect ratio).
  // Use 0.9 to leave a nice gap between the OSD and the edges of the window.
  float OSD_fScale;

  // Override Velocity of the Wind (WITHOUT gusts) at model position
  // Gusts will be added on top as selected in the wind config window
  // m/s (gusts are added on top in simulator)
  float Model_fNewWindVelX;
  float Model_fNewWindVelY;
  float Model_fNewWindVelZ;

  // Override Engine/Motor Revs per minute
  float Model_fNewEngine1_RPM;
  float Model_fNewEngine2_RPM;
  float Model_fNewEngine3_RPM;
  float Model_fNewEngine4_RPM;

  // Override Battery (electric motor models only)
  // V
  float  Model_fNewBatteryVoltage;
  // A
  float  Model_fNewBatteryCurrent;
  // Ah   Take care not to exceed the Model_fBatteryCapacity
  float  Model_fNewBatteryConsumedCharge;

  // Fuel (gas & jet engines)
  // l
  float  Model_fNewFuelConsumed;

  // Set to true to ignore colisions with terrain and wheel forces
  // Useful when setting the position of the model from the plugin
  // 0 = don't care; 1 = active; 2 = off
  unsigned char Model_bCrashInhibit;

};

#endif // DATATOAEROSIM_H
