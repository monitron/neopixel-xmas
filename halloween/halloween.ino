
#include <Adafruit_NeoPixel.h>


#define LEDS_PIN    6    // What output pin is the LED strip data line?
#define WIDTH       20   // How many LEDs are in one turn of the helix?
#define HEIGHT      8    // How many turns of the helix? Round up.
#define FRAME_DELAY 7    // Time between frames in ms (smaller is faster)
#define ANIM_FRAMES 20   // Number of frames between ember movements
#define FADE_FRAMES 10   // Number of frames to spend in transition (<= ANIM_FRAMES)
#define EMBER_RATE  75   // 100 = one new ember every cycle

#define COLOR_EMBER_R 255
#define COLOR_EMBER_G 70
#define COLOR_EMBER_B 0

#define COLOR_SKY_R  20
#define COLOR_SKY_G  0
#define COLOR_SKY_B  20

bool field[WIDTH][HEIGHT];
bool prevField[WIDTH][HEIGHT];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(WIDTH * HEIGHT, LEDS_PIN,
                                            NEO_GRB + NEO_KHZ800);
uint8_t frame = 0;

void setup()
{
  uint8_t x, y;

  randomSeed(analogRead(0));

  strip.begin();
  strip.setBrightness(64);
  strip.show();

  // Initialize lack of EMBER
  for(x = 0; x != WIDTH; x++) {
    for(y = 0; y != HEIGHT; y++) {
      field[x][y] = 0;
      prevField[x][y] = 0;
    }
  }
}

void loop()
{
  frame++;  
  if(frame >= ANIM_FRAMES) {
    frame = 0;
    moveEmbers();
  }
  drawField();
  delay(FRAME_DELAY);
}

void moveEmbers() {
  int8_t x, y;

  // Copy current field to prevField
  memcpy(prevField, field, sizeof(bool) * WIDTH * HEIGHT);

  // Move embers upwards
  for(y = HEIGHT - 1; y != -1; y--) {
    for(x = 0; x != WIDTH; x++) {
      if(field[x][y]) {
          field[x][y] = 0;
          if(y < HEIGHT - 1) field[x][y + 1] = 1;
      }
    }
  }

  if(random(0, 100) < EMBER_RATE) {
    // Seed a new ember
    field[random(0, WIDTH)][0] = 1;
  }
}

void drawField()
{
  uint8_t x, y;
  float frameF = frame;
  float progress = frameF / FADE_FRAMES;
  uint32_t color;
  uint8_t oldR, oldG, oldB, newR, newG, newB;

  if(progress > 1) progress = 1;

  for(x = 0; x != WIDTH; x++) {
    for(y = 0; y != HEIGHT; y++) {
      if(prevField[x][y]) {
        oldR = COLOR_EMBER_R;
        oldG = COLOR_EMBER_G;
        oldB = COLOR_EMBER_B;
      } else {
        oldR = COLOR_SKY_R;
        oldG = COLOR_SKY_G;
        oldB = COLOR_SKY_B;
      }
      if(field[x][y]) {
        newR = COLOR_EMBER_R;
        newG = COLOR_EMBER_G;
        newB = COLOR_EMBER_B;
      } else {
        newR = COLOR_SKY_R;
        newG = COLOR_SKY_G;
        newB = COLOR_SKY_B;
      }
      color = strip.Color(
        progress * newR + (1 - progress) * oldR,
        progress * newG + (1 - progress) * oldG,
        progress * newB + (1 - progress) * oldB
      );      
      strip.setPixelColor((y * WIDTH) + x, color);
    }
  }
  strip.show();
}

