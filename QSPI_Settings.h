#ifndef QSPI_SETTINGS_H
#define QSPI_SETTINGS_H

#include "MuxTest.h"
#include "daisysp.h"
#include "daisy_seed.h"
//#include "constants.h"

//holds alternative control settings, so they can be saved to flash
struct Settings
{
    float RevLength;
    float tapRatio;
    float ModDepth;
    float ModFreq;
    float HP_Cutoff;
    float LP_Cutoff;
    float Resonance;
    float FilterPrePost;
};

//extern Settings AltControls;

//save settings to flash memory
int SaveSettings(const Settings &currentSetting);

//load memory from flash memory
Settings LoadSettings();



#endif