#include "QSPI_Settings.h"
#include <string.h>
#include <math.h>

#define BUFF_SIZE (sizeof(Settings)/sizeof(float))

//block of memory in flash
static uint32_t DSY_QSPI_BSS membuff[(4 * BUFF_SIZE) + 1];   

uint32_t base = (uint32_t) &membuff[0];

//save data to flash memory
int SaveSettings(const Settings &currentSetting)
{
    float inbuff[BUFF_SIZE];

    inbuff[0] = currentSetting.RevLength;
    inbuff[1] = currentSetting.tapRatio;
    inbuff[2] = currentSetting.ModDepth;
    inbuff[3] = currentSetting.ModFreq;
    inbuff[4] = currentSetting.HP_Cutoff;
    inbuff[5] = currentSetting.LP_Cutoff;
    inbuff[6] = currentSetting.Resonance;
    inbuff[7] = currentSetting.FilterPrePost;
    
    //split into 8bit chunks:
    //skip first byte - out by one byte for some reason
    uint8_t writebuff[(4 * BUFF_SIZE) + 1];
    writebuff[0] = 0; //ignore first value?
    for(uint8_t i = 0; i < BUFF_SIZE; i++) 
    {
        uint8_t bytes[sizeof(float)];
        *(float*)(bytes) = inbuff[i];

        writebuff[(4*i) + 1] = bytes[0];
        writebuff[(4*i) + 2] = bytes[1];
        writebuff[(4*i) + 3] = bytes[2];
        writebuff[(4*i) + 4] = bytes[3];
    }

    hw.qspi_handle.mode = DSY_QSPI_MODE_INDIRECT_POLLING;
	dsy_qspi_init(&hw.qspi_handle);
    dsy_qspi_erase(base, base + ((BUFF_SIZE + 1) * sizeof(membuff[0])));
    return dsy_qspi_write(base, (BUFF_SIZE + 1) * sizeof(membuff[0]), (uint8_t*)writebuff);
}

//retreive data from flash memory
Settings LoadSettings()
{
    uint8_t outbuff[(4 * BUFF_SIZE) + 1];
    float readbuff[BUFF_SIZE];
    Settings SettingsInFlash{};

    hw.qspi_handle.mode = DSY_QSPI_MODE_DSY_MEMORY_MAPPED;
    dsy_qspi_init(&hw.qspi_handle);

    memcpy((void*)&outbuff,(void*)membuff,sizeof(outbuff));
    uint8_t bytes[sizeof(float)];

    for(uint8_t i = 0; i < BUFF_SIZE; i++)
    {
        for (uint8_t j = 0; j < sizeof(float); j++)
        {
            bytes[j] = outbuff[(4*i) + j];
        }
        readbuff[i] = *(float*)(bytes);
    }

SettingsInFlash.RevLength = readbuff[0];
SettingsInFlash.tapRatio = readbuff[1];
SettingsInFlash.ModDepth = readbuff[2];
SettingsInFlash.ModFreq = readbuff[3];
SettingsInFlash.HP_Cutoff = readbuff[4];
SettingsInFlash.LP_Cutoff = readbuff[5];
SettingsInFlash.Resonance = readbuff[6];
SettingsInFlash.FilterPrePost = readbuff[7];

return SettingsInFlash;

}