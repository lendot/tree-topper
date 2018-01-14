#include <Adafruit_NeoPixel.h>
 
#define PIN 1
#define NUM_LEDS 14
#define MIN_SATURATION 160

 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN);

typedef struct led {
  uint8_t h;
  int8_t h_dir;
  uint8_t s;
  int8_t s_dir;
} led;

led lights[NUM_LEDS];


void setup() {
  uint8_t i,j;
  randomSeed(analogRead(0));
  pixels.begin();
  pixels.setBrightness(255); // change brightness here (255=max!)
  for (i=0;i<NUM_LEDS;i++) {
    lights[i].h=random(256);
    lights[i].h_dir=random(2)+1;
    if (random(2)) lights[i].h_dir*=-1;
    lights[i].s=random(MIN_SATURATION,256);
    lights[i].s_dir=random(2)+1;
    if (random(2)) lights[i].s_dir*=-1;
  }
  pixels.show();
}


void
hsvtorgb(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t h, uint8_t s, uint8_t v)
{
    uint8_t region, fpart, p, q, t;
    
    if(s == 0) {
        *r = *g = *b = v;
        return;
    }
    

    region = h / 43;
 
    fpart = (h - (region * 43)) * 6;
    
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * fpart) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;
        
    switch(region) {
        case 0:
            *r = v; *g = t; *b = p; break;
        case 1:
            *r = q; *g = v; *b = p; break;
        case 2:
            *r = p; *g = v; *b = t; break;
        case 3:
            *r = p; *g = q; *b = v; break;
        case 4:
            *r = t; *g = p; *b = v; break;
        default:
            *r = v; *g = p; *b = q; break;
    }
    
    return;
}


 
void loop() {
  uint8_t i,j;
  uint8_t r,g,b;
  for (i=0;i<NUM_LEDS;i++) {
    int16_t c = lights[i].h;
    c+=lights[i].h_dir;
    if (c<0) {
      c=0;
      lights[i].h_dir=random(2)+1;
    }
    if (c>255) {
      c=255;
      lights[i].h_dir=-(random(2)+1);
    }
    lights[i].h=(uint8_t)c;

    c=lights[i].s;
    c+=lights[i].s_dir;
    if (c<MIN_SATURATION) {
      c=MIN_SATURATION;
      lights[i].s_dir=random(2)+1;
    }
    if (c>255) {
      c=255;
      lights[i].s_dir=-(random(2)+1);
    }
    lights[i].s=(uint8_t)c;
    
    hsvtorgb(&r,&g,&b,lights[i].h,lights[i].s,255);
    pixels.setPixelColor(i,r,g,b);

  }
  pixels.show();
  delay(100);
}
