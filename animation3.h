

#pragma pack(push,1)

typedef struct 
{
   int8_t index;
   uint16_t h;
   int16_t i;
   int8_t di;
   
} ANIDOT;


#define ANI3_DOTMAX (ANIN/2)

typedef struct 
{

    ANIDOT d[ANI3_DOTMAX];
    
} ANI3;

#pragma pack(pop)

#define ANI3  ( *((ANI3*)(_anibuf)) )



int ani3_enabledot()
{
    int n = random(ANIN);

    int i=0;
    int fuse=0;
    while (true)
    {

          for(int j = 0; j<ANI3_DOTMAX;j++)
            if ( (ANI3.d[j].di!=0) && ( (ANI3.d[j].index==i) || (ani_index_inc(ANI3.d[j].index)==i) || (ani_index_dec(ANI3.d[j].index)==i)  ) )
            {   i++;
                if (i>=ANIN)
                  i=0;
                fuse++;
                if (fuse>ANIN*3)
                {

                  Serial.println("!");
                  break;         
                }         
                continue;
            }

          if (n==0)
            break;
          n--;

          i = ani_index_inc(i);

          
    }

    //Serial.print("?");
    //Serial.println(i);

    return i;
        
}





void init_animation3(unsigned long t)
{
    static_assert(sizeof(ANI3) < ANIMATIONBUFSIZE, "ANI3 data structure too big");

    if (ANI.param0>ANI3_DOTMAX)
        ANI.param0 = ANI3_DOTMAX;
    if ( ANI.param0<1)
        ANI.param0 = 1;

    Serial.print("!");
    Serial.println(ANI.param0);        
    
    for(int j=0;j<ANI.param0;j++)
    {
         int i = ani3_enabledot();
         ANI3.d[j].index = i;
         ANI3.d[j].h = random(360);
         ANI3.d[j].i = random(1,INTENSIVITY_MAX);
         ANI3.d[j].di = random(1,3);
         if (random(100)>50)
            ANI3.d[j].di = (-1) * abs(ANI3.d[j].di);

    }


  
}

void loop_animation3(unsigned long t)
{
      ANIRGB c;
      ANIRGB cb={0};

      leds_setallblack();
      
      for(int j=0;j<ANI3_DOTMAX;j++)
      {
        
          if (ANI3.d[j].di==0)
              continue;


          HsvToRgb(ANI3.d[j].h,255,255,&c);

          c.r = (((int)c.r)*ANI3.d[j].i ) /INTENSIVITY_MAX;
          c.g = (((int)c.g)*ANI3.d[j].i ) /INTENSIVITY_MAX;
          c.b = (((int)c.b)*ANI3.d[j].i ) /INTENSIVITY_MAX;
          
          ani_led_set( ANI3.d[j].index, &c);
          
          ANI3.d[j].i+=ANI3.d[j].di;

          if (ANI3.d[j].i>INTENSIVITY_MAX)
          {
              ANI3.d[j].di = -1;
              ANI3.d[j].i = INTENSIVITY_MAX;
              continue;
          }

          if (ANI3.d[j].i<0)
          {
               ANI3.d[j].di = 0;
               int i = ani3_enabledot();
               ANI3.d[j].index = i;
               ANI3.d[j].h = random(360);
               ANI3.d[j].i = 0;
               ANI3.d[j].di = random(1,3);
          }
        
            
        
      }

}
