#ifndef DATAFROMAEROSIM_H
#define DATAFROMAEROSIM_H

#ifndef AEROSIMRC_MAX_CHANNELS
#define AEROSIMRC_MAX_CHANNELS 39
#endif

struct simToPlugin
{
    unsigned short structSize;
    float simTimeStep;
    float chSimTX[AEROSIMRC_MAX_CHANNELS];
    float chSimRX[AEROSIMRC_MAX_CHANNELS];
    unsigned char *OSDVideoBuf;
    unsigned long simMenuStatus;
    float initPosX;
    float initPosY;
    float initPosZ;
    float initHeading;
    float initPitch;
    float initRoll;
    float wpHomeX;
    float wpHomeY;
    float wpHomeLat;
    float wpHomeLong;
    const char *wpHomeDesc; // (m, deg, string)
    float wpAX;
    float wpAY;
    float wpALat;
    float wpALong;
    const char *wpADesc;    // (m, deg, string)
    float wpBX;
    float wpBY;
    float wpBLat;
    float wpBLong;
    const char *wpBDesc;    // (m, deg, string)
    float wpCX;
    float wpCY;
    float wpCLat;
    float wpCLong;
    const char *wpCDesc;    // (m, deg, string)
    float wpDX;
    float wpDY;
    float wpDLat;
    float wpDLong;
    const char *wpDDesc;    // (m, deg, string)
    float posX;
    float posY;
    float posZ;
    float velX;
    float velY;
    float velZ;
    float angVelX;
    float angVelY;
    float angVelZ;
    float accelX;
    float accelY;
    float accelZ;
    double latitude;
    double longitude;
    float AGL;
    float heading;
    float pitch;
    float roll;
    float windVelX;
    float windVelY;
    float windVelZ;
    float eng1RPM;
    float eng2RPM;
    float eng3RPM;
    float eng4RPM;
    float voltage;          // V
    float current;          // A
    float consumedCharge;   // Ah
    float capacity;         // Ah
    float fuelConsumed;            // l
    float fuelTankCapacity;        // l
}__attribute__((packed));

// normal - 592
// packed - 582

#endif // DATAFROMAEROSIM_H
