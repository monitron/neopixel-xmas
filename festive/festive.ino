#include <Adafruit_NeoPixel.h>

#define LEDS_PIN    6    // What output pin is the LED strip data line?
#define NUM_LEDS    150  // How many LEDs are there in the strip?
#define FRAME_DELAY 75   // Time between frames in ms (smaller is faster)
#define SCAN_FRAMES 150  // How many frames to stay on a single mode
#define MAX_MODE    12   // Number of the last mode

#define COLOR_ORANGE strip.Color(255, 100, 0)
#define COLOR_BLUE strip.Color(0, 0, 255)
#define COLOR_WHITE strip.Color(200, 200, 200)
#define COLOR_BLACK strip.Color(0, 0, 0)

uint8_t  mode;  // What display are we doing?
uint16_t frame; // How long have we been doing it?

uint32_t buffer[50];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN,
                                            NEO_GRB + NEO_KHZ800);

void setup()
{
  uint8_t i;

  randomSeed(analogRead(0));

  strip.begin();
  strip.setBrightness(64);
  strip.show();

  mode = 0;
  frame = 0;
}

void loop()
{
  switch(mode) {
  case 0:
    
    strip.setPixelColor(frame, COLOR_BLUE);
    break;
  case 1:
    strip.setPixelColor(frame, COLOR_ORANGE);
    break;
  case 2:
    strip.setPixelColor(frame, COLOR_WHITE);
    break;
  case 3:
    strip.setPixelColor(NUM_LEDS - frame, COLOR_BLUE);
    break;
  case 4:
    strip.setPixelColor(NUM_LEDS - frame, COLOR_ORANGE);
    break;
  case 5:
    strip.setPixelColor(NUM_LEDS - frame, COLOR_WHITE);
    break;
  case 6: // Candy stripes
    if(frame == 0) {
      fillBufferSolid(0, 10, COLOR_WHITE);
      fillBufferSolid(10, 10, COLOR_ORANGE);
      fillBufferSolid(20, 10, COLOR_BLACK);
    }
    drawMarquee(30);
    break;
  case 7: // Thin blue stripes
    if(frame == 0) {
      fillBufferSolid(0, 3, COLOR_BLUE);
      fillBufferSolid(3, 7, COLOR_BLACK);
    }
    drawMarquee(10);
    break;
  case 8: // Thin orange stripes
    if(frame == 0) {
      fillBufferSolid(0, 3, COLOR_ORANGE);
      fillBufferSolid(3, 7, COLOR_BLACK);
    }
    drawMarquee(10);
    break;
  case 9: // Red/black gradient stripes
    if(frame == 0) {
      fillBufferGradient(0,  10, 0,   0, 0, 255, 0, 0);
      fillBufferGradient(10, 10, 255, 0, 0, 0,   0, 0);
    }
    drawMarquee(20);
    break;
  case 10: // Blue/black gradient stripes
    if(frame == 0) {
      fillBufferGradient(0,  10, 0,   0, 0, 0, 0, 255);
      fillBufferGradient(10, 10, 0, 0, 255, 0, 0,   0);
    }
    drawMarquee(20);
    break;
  case 11: // White/black gradient stripes
    if(frame == 0) {
      fillBufferGradient(0,  10, 0, 0, 0, 255, 255, 255);
      fillBufferGradient(10, 10, 255, 255, 255, 0, 0, 0);
    }
    drawMarquee(20);
    break;
  case 12: // White/blue gradient stripes
    if(frame == 0) {
      fillBufferGradient(0,  20, 0, 0, 255, 255, 255, 255);
      fillBufferGradient(20, 20, 255, 255, 255, 0, 0, 255);
    }
    drawMarquee(40);
    break;
  }

  strip.show();
  frame++;
  delay(FRAME_DELAY);
  if(frame > SCAN_FRAMES) {
    mode = random(MAX_MODE + 1);
    frame = 0;
  }
}

void fillBufferSolid(uint8_t start, uint8_t size, uint32_t color)
{
  uint8_t i;
  for(i = start; i != (start + size); i++) {
    buffer[i] = color;
  }
}

void fillBufferGradient(uint8_t start, uint8_t size,
                        uint8_t r0, uint8_t g0, uint8_t b0,
                        uint8_t r1, uint8_t g1, uint8_t b1)
{
  uint8_t i;
  float sizeF = size;
  for(i = 0; i != size; i++) {
    buffer[i + start] = strip.Color((r0 * ((size - i) / sizeF)) + (r1 * (i / sizeF)),
                                    (g0 * ((size - i) / sizeF)) + (g1 * (i / sizeF)),
                                    (b0 * ((size - i) / sizeF)) + (b1 * (i / sizeF)));
  }
}

void drawMarquee(uint8_t size)
{
  uint8_t i;
  for(i = 0; i != NUM_LEDS; i++) {
    strip.setPixelColor(i, buffer[(i + frame) % size]);
  }
}

void drawSolid(uint32_t color)
{
  uint8_t i;
  for(i = 0; i != NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
}
