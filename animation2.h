
#define ANI2_OFF 0
#define ANI2_R   1
#define ANI2_G   2
#define ANI2_B   3

#pragma pack(push,1)

typedef struct 
{
   uint8_t index1;
   uint8_t index2;
   uint16_t intensivity;
   uint8_t color;
   uint8_t changespeed;
} ANIPAIR;


#define ANI2_PAIRSCOUNT (ANIN/2)

typedef struct 
{

    ANIPAIR p[ANI2_PAIRSCOUNT];
    
} ANI2;

#pragma pack(pop)

#define ANI2  ( *((ANI2*)(_anibuf)) )

int ani2_getrandomindex()
{
    int n = random(ANIN);

    int i=0;
    int fuse=0;
    while (true)
    {
        for(int j=0;j<ANI2_PAIRSCOUNT;j++)

          if ( (ANI2.p[j].color!=ANI2_OFF) && (
            (i==ANI2.p[j].index1) || 
            (i==ani_index_inc(ANI2.p[j].index1)) ||
            (i==ani_index_dec(ANI2.p[j].index1)) ||
            (i==ANI2.p[j].index2) ||
            (i==ani_index_inc(ANI2.p[j].index2)) ||
            (i==ani_index_dec(ANI2.p[j].index2)) 
            
            ))
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

    return i;
    
    
}


void init_animation2(unsigned long t)
{
    static_assert(sizeof(ANI2) < ANIMATIONBUFSIZE, "ANI2 data structure too big");
    int i=0;
    for(int j=0;j<ANI.param0;j++)
    {
          ANI2.p[i].index1 = ani2_getrandomindex();
          ANI2.p[i].index2 = ani2_getrandomindex();
          ANI2.p[i].intensivity = 0;
          ANI2.p[i].color = ANI2_R;
          ANI2.p[i].changespeed = random(1,5);
          i++;
          ANI2.p[i].index1 = ani2_getrandomindex();
          ANI2.p[i].index2 = ani2_getrandomindex();
          ANI2.p[i].intensivity = 0;
          ANI2.p[i].color = ANI2_G;
          ANI2.p[i].changespeed = random(1,5);
          i++;
          ANI2.p[i].index1 = ani2_getrandomindex();
          ANI2.p[i].index2 = ani2_getrandomindex();
          ANI2.p[i].intensivity = 0;
          ANI2.p[i].color = ANI2_B;
          ANI2.p[i].changespeed = random(1,5);
          i++;
    }

    


  
}

void loop_animation2(unsigned long t)
{

      leds_setallblack();

      ANIRGB c1,c2;
      for(int j=0;j<ANI2_PAIRSCOUNT;j++)
      {
           if (ANI2.p[j].color==ANI2_OFF)
              continue;

          uint8_t intensivity1 = ANI2.p[j].intensivity;   
          uint8_t intensivity2 = INTENSIVITY_MAX - ANI2.p[j].intensivity;

          memset(&c1,0,sizeof(ANIRGB));
          memset(&c2,0,sizeof(ANIRGB));
          switch (ANI2.p[j].color)
          {   case ANI2_R:
                c1.r=intensivity1;
                c2.r=intensivity2;                
                break;
              case ANI2_G:
                c1.g=intensivity1;
                c2.g=intensivity2;                
                break;
              case ANI2_B:
                c1.b=intensivity1;
                c2.b=intensivity2;                
                break;
          }       
        
          ani_led_set( ANI2.p[j].index1, &c1);  
          ani_led_set( ANI2.p[j].index2, &c2);  

          ANI2.p[j].intensivity+=ANI2.p[j].changespeed;

          if (ANI2.p[j].intensivity>=INTENSIVITY_MAX)
          {
              ANI2.p[j].intensivity=0;
              ANI2.p[j].index2 = ANI2.p[j].index1;
              ANI2.p[j].index1 = ani2_getrandomindex();
              ANI2.p[j].changespeed = random(1,5);
            
          }
          
      }
}
