typedef struct 
{
    float hh  ;
    float dh ;
    int skipcount;

    int offset;
    int offset_i;
    
} ANI1;

#define ANI1  ( *((ANI1*)(_anibuf)) )


void init_animation1(unsigned long t)
{
      static_assert(sizeof(ANI1) < ANIMATIONBUFSIZE, "ANI1 data structure too big");
 
  
      ANI1.hh =0.0;
      ANI1.dh = ((float)ANI.param0)/ANIN;
      ANI1.skipcount = ANI.param1;
      if (ANI1.skipcount<=0)
          ANI1.skipcount =1;

      ANI1.offset = 0;
      ANI1.offset_i = 0;

      

      
}

void loop_animation1(unsigned long t)
{
  
      float h = ANI1.hh;
      ANIRGB c;
      ANIRGB cb = {0};

      leds_setallblack();
      
      int pos=ANI1.offset;
      for(int j = 0; j < ANIN; j++)
      {

        
          if ((j % ANI1.skipcount)  != 0)
          {
              //ani_led_set( pos, &cb);

          } else
          {
              HsvToRgb(h,255,255,&c);


              if (ANI1.skipcount != 1)
              {
                  int pos1 = pos;
                  int pos2 =   ani_index_inc(pos);
                  ANIRGB c1 = {c.r,c.g,c.b};
                  ANIRGB c2 = {c.r,c.g,c.b};
                  ani_add_intensivity(ANI1.offset_i,&c1);
                  ani_add_intensivity(INTENSIVITY_MAX -  ANI1.offset_i,&c2);
                  ani_led_set( pos1, &c2); 
                  ani_led_set( pos2, &c1); 
                   
              } else
              {
                  ani_led_set( pos, &c);  
              }
          }

          pos = ani_index_inc(pos);
          
          h+=ANI1.dh;
          if (h>360.0)
            h-=360;
          
      }

      ANI1.hh+=1;
      if (ANI1.hh>360.0)
        ANI1.hh-=360.0;

      ANI1.offset_i++;
      if (ANI1.offset_i>INTENSIVITY_MAX)
      {
         ANI1.offset_i = 0;
         ANI1.offset = ani_index_inc(ANI1.offset);
      }


  
}
