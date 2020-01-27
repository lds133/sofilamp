#define POWERLED_SETTIMEOUT 1000
#define POWERLED_BLINKTIMEOUT 500
#define POWERLED_MIN 5

int _powerled = 0;
unsigned long _powerledtime = 0;
unsigned long _powerledsettimeout = 0;

void powerled_init()
{
    _powerled = 255;
    pinMode(PIN_LEDPOWER,OUTPUT);
    analogWrite(PIN_LEDPOWER, _powerled);
    _powerledtime = millis();
    _powerledsettimeout = POWERLED_SETTIMEOUT;
}




void powerled_loop(unsigned long t,int ldr)
{

    if (ldr>LDR_TOOMUCHLIGHT )
    {    _powerled = 255;
    } else
    {   unsigned long v = (((unsigned long)ldr)*255)/LDR_TOOMUCHLIGHT ;
        _powerled = v;
        if (_powerled<POWERLED_MIN)
            _powerled = POWERLED_MIN;
    }


    if ((t-_powerledtime)>_powerledsettimeout)
    {
        analogWrite(PIN_LEDPOWER, _powerled);
        _powerledtime = t;
        _powerledsettimeout = POWERLED_SETTIMEOUT;
        //Serial.print("P");
        //Serial.println(_powerled);
    }
  
}

void powerled_blink(unsigned long t)
{
    analogWrite(PIN_LEDPOWER, 0);
    _powerledtime = t;  
    _powerledsettimeout = POWERLED_BLINKTIMEOUT;
}
