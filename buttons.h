
typedef struct 
{
    byte Pin;
    unsigned long Time;
    byte LastState;
    byte CurrState;    
    int ClickCounter;
    boolean IsHold;    
    
} BUTTON;

BUTTON _btnA={0};
BUTTON _btnB={0};



void btn_prepare(BUTTON* pb,unsigned long t);
void btn_process(BUTTON* pb,unsigned long t);


void btn_init()
{
  pinMode(PIN_BUTTON1,INPUT);
  pinMode(PIN_BUTTON2,INPUT);
  digitalWrite(PIN_BUTTON1,HIGH);
  digitalWrite(PIN_BUTTON2,HIGH);
  
  _btnA.Pin = PIN_BUTTON1;
  _btnB.Pin = PIN_BUTTON2;

  unsigned long t=millis();

  btn_prepare(&_btnA,t);
  btn_prepare(&_btnB,t);

}



void btn_check(unsigned long t)
{
  btn_process(&_btnA,t);
  btn_process(&_btnB,t);
}


void btn_prepare(BUTTON* pb,unsigned long t)
{
    pb->Time = t;
    pb->LastState = digitalRead(pb->Pin);
    pb->ClickCounter=0;
    pb->IsHold = false;  
    pb->CurrState = digitalRead(pb->Pin);    
}


#define DEBOUNCETIME 5
#define HOLDTIME 5000
void btn_process(BUTTON* pb,unsigned long t)
{
    byte state = digitalRead(pb->Pin);
  
 
    if (pb->LastState!=state) 
    {    pb->Time = t;
         pb->LastState = state;
         pb->IsHold = false;
    }
    
    if ( (t-pb->Time>DEBOUNCETIME) && (pb->CurrState!=state) )
    {   if (state == LOW) 
        {  pb->ClickCounter++;
        } 
        pb->CurrState = state;
        pb->LastState = state;
    }
    
    if ( (!pb->IsHold) && (t-pb->Time>HOLDTIME) && (pb->CurrState == LOW) )  
    {  pb->IsHold = true;
    }
}
