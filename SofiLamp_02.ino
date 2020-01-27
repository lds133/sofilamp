#include <avr/wdt.h>




#define LED_COUNT 24

#define PIN_LEDDATA  2
#define PIN_LEDRED     LED_BUILTIN
#define PIN_LEDPOWER   9
#define PIN_BUTTON1    4
#define PIN_BUTTON2    5
#define PIN_LDR        A3

#define LOOP_DELAY_MS  5    
#define INTENSIVITY_MAX 255

#define LDR_TOOMUCHLIGHT  500
#define LDR_DARK          400
#define LDR_REALDARK      200

#include "settings.h"
#include "LDR.h"
#include "buttons.h"
#include "ledstrip.h"
#include "powerled.h"

#include "animation_prefix.h"
#include "animation0.h"
#include "animation1.h"
#include "animation2.h"
#include "animation3.h"
#include "animation.h"


#define INTENSIVITY_STEP (INTENSIVITY_MAX/7)

#define SLEEP_TIME_MINS 45


#define STATE_WORKING       1
#define STATE_TOOMUCHLIGHT  2
#define STATE_SLEEPING      3

unsigned long _sleeptime = 0;
unsigned long _t=0;
unsigned long _ontime=0;
int _state;

void setup()
{
    Serial.begin(115200);
    Serial.println("*SofiLamp* ver 2.1 JAN 2020");

    
    settings_init();
    ldr_init();
    btn_init();
    leds_init(INTENSIVITY_MAX);
    init_animation(_cfg.animationid,INTENSIVITY_MAX);
    powerled_init();


    wdt_enable(WDTO_4S);

    
    _sleeptime = 1000L*60*SLEEP_TIME_MINS;
    set_state(STATE_WORKING);
    _ontime =millis();
    
}

void set_state(int newstate)
{
      Serial.print("M");
      Serial.println(newstate);
      _state = newstate;
}




bool checkbutton(unsigned long t,BUTTON* pb, int changemode)
{
    int n = pb->ClickCounter;
    pb->ClickCounter=0;
    
    if (n!=0)
    {   
        if (_state == STATE_SLEEPING)
        {   set_state(STATE_TOOMUCHLIGHT);
            return;
        }
      
        int id = _aniindex;
        for(int j=0;j<n;j++)
        {   id = change_animation(changemode);
        }
        if (id!=_cfg.animationid)
        {  _cfg.animationid = id;
           settings_setdirty(t);
        }
        pb->ClickCounter=0;
        powerled_blink(t);
        _ontime =t;
    }
} 



void loop()
{

    unsigned long t = millis();

    wdt_reset();

    
    
    settings_loop(t);
    
    btn_check(t);
    int ldr = ldr_get(t);
    powerled_loop(t,ldr);

    if (_state == STATE_WORKING)
    {   if (ldr>LDR_TOOMUCHLIGHT)
        {     turnoff_animation();
              set_state(STATE_TOOMUCHLIGHT);
        } else
        {   if ((t -_ontime)>_sleeptime)
            {   turnoff_animation();
                set_state(STATE_SLEEPING);
            }
            if (ldr>LDR_REALDARK)
            {   _ontime =t;
            }
            
        }
    } else
    if (_state == STATE_TOOMUCHLIGHT)
    {
          if (ldr<LDR_DARK )
          {   _ontime = t;
              set_state(STATE_WORKING);
              turnon_animation();
          }
    } else
    if (_state == STATE_SLEEPING)
    {   if (ldr>LDR_TOOMUCHLIGHT)
            set_state(STATE_TOOMUCHLIGHT);
    }

    loop_animation(t);
    leds_show();

    
    checkbutton(t,&_btnB,ANI_CHANGE_SLAVE);
    
    checkbutton(t,&_btnA,ANI_CHANGE_MASTER);

    
    if ((t-_t)>1000)
    {
        Serial.print("L");
        Serial.println(ldr);
        _t =t;
        digitalWrite(PIN_LEDRED, HIGH);
    }

    
    delay(LOOP_DELAY_MS);
    digitalWrite(PIN_LEDRED, LOW);
}
