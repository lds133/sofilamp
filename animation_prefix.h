typedef struct 
{   uint8_t r;
    uint8_t g;
    uint8_t b;
      
} ANIRGB;



typedef void (*animation_fnc_ptr)(unsigned long t);

#define ANIMATIONBUFSIZE 110

#pragma pack(push,1)
typedef struct tagANIMATION
{
    uint8_t next_animation_index_master;
    uint8_t next_animation_index_slave;
    animation_fnc_ptr init_animation_fnc;
    animation_fnc_ptr loop_animation_fnc;
    uint8_t frametime;
    int  param0;
    uint8_t param1;
}  ANIMATION;

uint8_t _anibuf[ANIMATIONBUFSIZE];

#pragma pack(pop)


int _aniindex=0;
int _aniframecounter=0;
unsigned long _anitime=0;

int _aniintensivity=0; 
int _aniintensivitytarget=0;
unsigned long _aniintensivitytime=0;
const unsigned long _aniturndelay_ms = 100;


#define ANI_INTENSIVITY_CHANGE_STEP   10

#define ANI_CHANGE_MASTER  1
#define ANI_CHANGE_SLAVE   2


void ani_add_intensivity(int i, ANIRGB* c);
int change_animation(int changemode);
void ani_led_set(int index, ANIRGB* c);
ANIMATION* getcurrent_animation();


#define ANI (*(getcurrent_animation()))

#define ANIN LED_COUNT



int ani_index_inc(int n)
{   n++;
    if (n>=ANIN)
      n=0;
    return n;
}


int ani_index_dec(int n)
{   n--;
    if (n<0)
      n=ANIN-1;
    return n;
}

void HsvToRgb(uint16_t h, uint8_t s, uint8_t v, ANIRGB* c)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    c->r = r;
    c->g = g;
    c->b = b;
}
