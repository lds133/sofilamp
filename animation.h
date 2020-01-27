

#define AG0  2
#define AG1  (AG0+6)
#define AG2  (AG1+6)
#define AG3  (AG2+6)


ANIMATION _animations[] = 
  {   
      {  1, 0, NULL                     ,  NULL                     , 0  ,  0  ,0 },            //0 
      
      {  AG0, 2, &init_animation_startup  ,  &loop_animation_startup      , 100,  360,0 },      //1
      
      {  AG1, AG0+1, &init_animation1         ,  &loop_animation1         , 100,  360,6  }, 
      {  AG1, AG0+2, &init_animation1         ,  &loop_animation1         , 100,  360,4  }, 
      {  AG1, AG0+3, &init_animation1         ,  &loop_animation1         , 100,  360,3 }, 
      {  AG1, AG0+4, &init_animation1         ,  &loop_animation1         , 100,  360,2 }, 
      {  AG1, AG0+5, &init_animation1         ,  &loop_animation1         , 10,   360,1 }, 
      {  AG1, AG0  , &init_animation1         ,  &loop_animation1         , 1,    360,1 }, 

      {  AG2, AG1+1, &init_animation1         ,  &loop_animation1         , 10,   180 ,6  }, 
      {  AG2, AG1+2, &init_animation1         ,  &loop_animation1         , 100,  90  ,4  },       
      {  AG2, AG1+3, &init_animation1         ,  &loop_animation1         , 50,   180 ,3 },       
      {  AG2, AG1+4, &init_animation1         ,  &loop_animation1         , 100,  90  ,3 },       
      {  AG2, AG1+5, &init_animation1         ,  &loop_animation1         , 50,   180 ,1 },       
      {  AG2, AG1  , &init_animation1         ,  &loop_animation1         , 150,  90  ,1 },       
      
      {  AG3, AG2+1, &init_animation2         ,  &loop_animation2         , 50,   1  ,0 }, 
      {  AG3, AG2+3, &init_animation2         ,  &loop_animation2         , 50,   2  ,0 }, 
      {  AG3, AG2+4, &init_animation2         ,  &loop_animation2         , 100,  2  ,0 }, 
      {  AG3, AG2+5, &init_animation2         ,  &loop_animation2         , 50,   3  ,0 }, 
      {  AG3, AG2+6, &init_animation2         ,  &loop_animation2         , 150,  3  ,0 }, 
      {  AG3, AG2  , &init_animation2         ,  &loop_animation2         , 150,  4  ,0 }, 
      
      {  AG0, AG3+1, &init_animation3         ,  &loop_animation3         , 10,   1  ,0 }, 
      {  AG0, AG3+2, &init_animation3         ,  &loop_animation3         , 100,  2  ,0 },       
      {  AG0, AG3+3, &init_animation3         ,  &loop_animation3         , 150,  3  ,0 }, 
      {  AG0, AG3+4, &init_animation3         ,  &loop_animation3         , 100,  5  ,0 },       
      {  AG0, AG3  , &init_animation3         ,  &loop_animation3         , 50,   8  ,0 },       

      
  };


#define ANI_STARTUPID  1

ANIMATION* getcurrent_animation()
{
    return &_animations[_aniindex];
}






void ani_led_set(int index, ANIRGB* c)
{
    ANIRGB cc = {c->r,c->g,c->b};
    ani_add_intensivity(_aniintensivity,&cc);
    leds_set(index,cc.r,cc.g,cc.b);
}

void ani_add_intensivity(int i, ANIRGB* c)
{
    c->r = (((int)c->r)*i ) /INTENSIVITY_MAX;
    c->g = (((int)c->g)*i ) /INTENSIVITY_MAX;
    c->b = (((int)c->b)*i ) /INTENSIVITY_MAX;
}


void init_animation(int id,int intensivity)
{

    _animations[ANI_STARTUPID]. next_animation_index_master = id;
    _aniindex=0;
    _aniintensivity=0;
    _aniintensivitytarget=INTENSIVITY_MAX;
    _aniintensivitytime=millis();
    change_animation(ANI_CHANGE_MASTER);
  
}

void turnon_animation()
{
    _aniintensivitytarget=INTENSIVITY_MAX;
    _aniintensivitytime=millis();
}

void turnoff_animation()
{
    _aniintensivitytarget=0;
    _aniintensivitytime=millis();
}


int change_animation(int changemode)
{


    unsigned long t = millis();
    if (changemode == ANI_CHANGE_MASTER)
    {   _aniindex=ANI.next_animation_index_master;
    } else
    {   _aniindex=ANI.next_animation_index_slave;
    }
    _aniframecounter=0;
    _anitime=t;
    memset(_anibuf,0,ANIMATIONBUFSIZE);
    ANI.init_animation_fnc(t);  
    Serial.print("A");
    Serial.println(_aniindex);

    return _aniindex;
}

void loop_animation(unsigned long t)
{
    if ((_aniintensivity!=_aniintensivitytarget) && (t-_aniintensivitytime)>_aniturndelay_ms)
    {
          if (_aniintensivity>_aniintensivitytarget)
          {   _aniintensivity-= ANI_INTENSIVITY_CHANGE_STEP;
              if (_aniintensivity<=0)
                  _aniintensivity =0; 
          } else
          {   _aniintensivity+= ANI_INTENSIVITY_CHANGE_STEP;
              if (_aniintensivity>=INTENSIVITY_MAX)
                  _aniintensivity =INTENSIVITY_MAX; 
          }
          _aniintensivitytime = t;
    }

    if ((t-_anitime)>ANI.frametime)
    {  

        _aniframecounter++;
        if (_aniintensivity==0)
        {   leds_setallblack();
        } else
        {   ANI.loop_animation_fnc(t);
        }

        _anitime = t;
    }
    
}
