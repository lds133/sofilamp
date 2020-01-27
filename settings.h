#include <EEPROM.h>


#define SETTINGS_ADDR     0
#define SETTINGS_NOTEMPTY 0x02

#define SETTINGS_WRITE_TIMEOUT  10000

#pragma pack(push,1)

typedef struct 
{
    uint8_t flag;
    uint16_t animationid;
} SETTINGS;

unsigned long _cfgtime = 0;
bool _cfgisdirty = false;
SETTINGS _cfg = {0}; 

#pragma pack(pop)

void settings_init()
{
    int a = SETTINGS_ADDR;
    uint8_t* v = (uint8_t*)&_cfg;
    for(int i=0;i<sizeof(SETTINGS);i++)
       v[i] = EEPROM.read(a++);    
    if (_cfg.flag != SETTINGS_NOTEMPTY)
    {
         _cfg.animationid = 2;
         _cfg.flag = SETTINGS_NOTEMPTY;
    }    

    _cfgtime = millis();
    _cfgisdirty = false;
}

void settings_setdirty(unsigned long t)
{
    _cfgtime = t;
    _cfgisdirty = true;
}

void settings_loop(unsigned long t)
{
    if ( (_cfgisdirty ) && ((t - _cfgtime)>SETTINGS_WRITE_TIMEOUT))
    {
        int a = SETTINGS_ADDR;
        uint8_t* v = (uint8_t*)&_cfg;
        for(int i=0;i<sizeof(SETTINGS);i++)
           EEPROM.write(a++,v[i]);
        _cfgisdirty = false;
        Serial.println("S");
      
    }
  
}
