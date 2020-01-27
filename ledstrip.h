#include <PololuLedStrip.h>

PololuLedStrip<PIN_LEDDATA> _strip;
rgb_color _leds[LED_COUNT];

uint8_t _ledsintensivity=0;







void leds_setintensivity(int intensivity)
{
     _ledsintensivity=intensivity;
     Serial.print("I");
     Serial.println(intensivity);
}

void leds_init(int intensivity)
{
     leds_setintensivity(intensivity);
}

void leds_show()
{   _strip.write(_leds, LED_COUNT);
}

void leds_set(int index,uint8_t r,uint8_t g,uint8_t b)
{   _leds[index].red = (((int)r)*_ledsintensivity)/INTENSIVITY_MAX;
    _leds[index].green = (((int)g)*_ledsintensivity)/INTENSIVITY_MAX;
    _leds[index].blue = (((int)b)*_ledsintensivity)/INTENSIVITY_MAX; 
}

void leds_setall(uint8_t r,uint8_t g,uint8_t b)
{   for(int i = 0; i < LED_COUNT; i++)
      leds_set( i, r, g, b );
}

void leds_setallblack()
{   leds_setall(0,0,0);
}
