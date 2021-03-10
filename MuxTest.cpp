#include "MuxTest.h"
//#include "daisy_seed.h"
#include "QSPI_Settings.h"
#include <string.h>
#include <atomic>


Settings setting{};
constexpr Settings defaultSetting
{
    (25000.0f + 240000.0f) / 2.0f, //RevLength
    1.0f,   //tapRatio
    0.0f,   //ModDepth
    (10.0f + 0.1f) / 2.0f,   //ModFreq
    200.0f,   //HP_Cutoff
    12000.0f,   //LP_Cutoff
    0.0f,     //Filter Resonance
    0.0f
};

float pot1,pot2,pot3,pot4,pot5,pot6,pot7,pot8;
float CV1,CV2,CV3,CV4,CV5,CV6,CV7,CV8;

Led led1,led2;

std::atomic<bool> save_flag{};

static void AudioCallback(float *in, float *out, size_t size)
{
     
    led1.Update();
    led2.Update();
    static uint32_t timeSinceLast{};

    //try to save every 5 seconds if not currently saving
    if( ( (System::GetNow() - timeSinceLast) > 5000) && !save_flag )  
    {
        save_flag = true;
        timeSinceLast = System::GetNow(); //reset timer
    }

    
    for(size_t i = 0; i < size; i += 2)
    {   
       out[i] =  in[i];
       out[i+1] = in[i+1];
    }

}

int main(void)
{
    hw.Configure();
    hw.Init();
    led1.Init(hw.GetPin(24), false, hw.AudioSampleRate());
    led2.Init(hw.GetPin(10),false, hw.AudioSampleRate());

    Settings setting2{LoadSettings()};
    led2.Set(setting2.FilterPrePost);
    
    AdcChannelConfig adcConfig[8];

//CV inputs
    adcConfig[0].InitSingle(hw.GetPin(15));
    adcConfig[1].InitSingle(hw.GetPin(16));
    adcConfig[2].InitSingle(hw.GetPin(17));
    adcConfig[3].InitSingle(hw.GetPin(18));
    adcConfig[4].InitSingle(hw.GetPin(19));
    adcConfig[5].InitSingle(hw.GetPin(20));
    adcConfig[6].InitSingle(hw.GetPin(22));

//Muxed pot inputs
    adcConfig[7].InitMux(hw.GetPin(25),8,hw.GetPin(14),hw.GetPin(13),hw.GetPin(12));

    hw.adc.Init(adcConfig, 8);
    hw.adc.Start();

    hw.SetAudioBlockSize(1);     //set blocksize.
    hw.StartAudio(AudioCallback);

    // Loop forever
    for(;;)
    {
        pot1 = hw.adc.GetMuxFloat(7,0); 
        setting.FilterPrePost = pot1;
        pot2 = hw.adc.GetMuxFloat(7,1);
        pot3 = hw.adc.GetMuxFloat(7,2);
        pot4 = hw.adc.GetMuxFloat(7,3);
        pot5 = hw.adc.GetMuxFloat(7,4); 
        pot6 = hw.adc.GetMuxFloat(7,5); 
        pot7 = hw.adc.GetMuxFloat(7,6); 
        pot8 = hw.adc.GetMuxFloat(7,7); 
 
        CV1 = hw.adc.GetFloat(0); 
        CV2 = hw.adc.GetFloat(1); 
        CV3 = hw.adc.GetFloat(2); 
        CV4 = hw.adc.GetFloat(3); 
        CV5 = hw.adc.GetFloat(4); 
        CV6 = hw.adc.GetFloat(5); 
        CV7 = hw.adc.GetFloat(6); 

        led1.Set(pot1);
        
        if(save_flag)   //if save_flag set save settings
        {
            if (SaveSettings(setting) == DSY_MEMORY_OK)
            {
                save_flag = false;
            }
        }
    }
}