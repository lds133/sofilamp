typedef struct 
{

    unsigned long  t;
    int stage;
    int oldintensivity;
    
} ANI0;



#define ANI0  ( *((ANI0*)(_anibuf)) )

#define ANI0_STAGE_TIMEOUT 1000
#define ANI0_STAGE_R 0
#define ANI0_STAGE_G 1
#define ANI0_STAGE_B 2
#define ANI0_STAGE_BLACK 3


void init_animation_startup(unsigned long t)
{
    static_assert(sizeof(ANI0) < ANIMATIONBUFSIZE, "ANI0 data structure too big");
    ANI0.t = t;
    ANI0.stage = ANI0_STAGE_R;
    ANI0.oldintensivity = _aniintensivity;

}

void loop_animation_startup(unsigned long t)
{

      if ((t-ANI0.t)>ANI0_STAGE_TIMEOUT)
      {
          ANI0.t = t;
          ANI0.stage++;
      }

      ANIRGB c = {0};


      switch( ANI0.stage )
      {
          case    ANI0_STAGE_R:
              c.r =255;
              _aniintensivity=INTENSIVITY_MAX;
              _aniintensivitytarget=INTENSIVITY_MAX;              
              break;
          case    ANI0_STAGE_G:
              c.g =255;
              break;
          case    ANI0_STAGE_B:
              c.b =255;
              break;   
          case    ANI0_STAGE_BLACK:
  
              break;   


          default:
              change_animation(ANI_CHANGE_MASTER);
              _aniintensivity=0;
              _aniintensivitytarget=INTENSIVITY_MAX;               
              return;
                             
      }


      for(int i = 0; i < ANIN; i++)
          ani_led_set( i, &c);
      
}
