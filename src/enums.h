#ifndef ENUMS_H
#define ENUMS_H

// Custom Menu Item masks
enum MenuMasks {
    MenuCmdReset    = (1 << 0),
    MenuEnable      = (1 << 1),
    MenuTx          = (1 << 2),
    MenuRx          = (1 << 3),
    MenuLedBlue     = (1 << 4),
    MenuLedGreen    = (1 << 5),
    MenuFMode1      = (1 << 6),
    MenuFMode2      = (1 << 7),
    MenuFMode3      = (1 << 8),
    MenuScreen      = (1 << 9),
    MenuNextWpt     = (1 << 10)
};

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

enum Channels {
    Ch1Aileron,
    Ch2Elevator,
    Ch3Throttle,
    Ch4Rudder,
    Ch5,
    Ch6,
    Ch7,
    Ch8,
    Ch9,
    Ch10Retracts,
    Ch11Flaps,
    Ch12FPVPan,
    Ch13FPVTilt,
    Ch14Brakes,
    Ch15Spoilers,
    Ch16Smoke,
    Ch17Fire,
    Ch18FlightMode,
    Ch19ALTHold,
    Ch20FPVTiltHold,
    Ch21ResetModel,
    Ch22MouseTX,
    Ch23Plugin1,
    Ch24Plugin2,
    Ch25ThrottleHold,
    Ch26CareFree,
    Ch27,
    Ch28LMotorDual,
    Ch29RMotorDual,
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

#endif // ENUMS_H
