
#include <Adafruit_NeoPixel.h>


#define LEDS_PIN    6    // What output pin is the LED strip data line?
#define WIDTH       20   // How many LEDs are in one turn of the helix?
#define HEIGHT      8    // How many turns of the helix? Round up.
#define FRAME_DELAY 10   // Time between frames in ms (smaller is faster)
#define ANIM_FRAMES 20   // Number of frames between snow movements
#define FADE_FRAMES 10   // Number of frames to spend in transition (<= ANIM_FRAMES)
#define CYCLES      200  // Reset after this many snow movements
#define FLAKE_RATE  50   // 100 = one new snowflake every cycle

#define COLOR_SNOW_R 200
#define COLOR_SNOW_G 200
#define COLOR_SNOW_B 200

#define COLOR_SKY_R  10
#define COLOR_SKY_G  10
#define COLOR_SKY_B  40

bool snow[WIDTH][HEIGHT];
bool prevSnow[WIDTH][HEIGHT];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(WIDTH * HEIGHT, LEDS_PIN,
                                            NEO_GRB + NEO_KHZ800);
uint8_t frame = 0;
uint8_t cycle = 0;

void setup()
{
  uint8_t x, y;

  randomSeed(analogRead(0));

  strip.begin();
  strip.setBrightness(64);
  strip.show();

  // Initialize lack of snow
  for(x = 0; x != WIDTH; x++) {
    for(y = 0; y != HEIGHT; y++) {
      snow[x][y] = 0;
      prevSnow[x][y] = 0;
    }
  }
}

void loop()
{
  frame++;  
  if(frame >= ANIM_FRAMES) {
    frame = 0;
    cycle++;
    if(cycle >= CYCLES) {
      cycle = 0;
      clearSnow();
    } else {
      moveSnow();
    }
  }
  drawSnow();
  delay(FRAME_DELAY);
}

void moveSnow() {
  uint8_t x, y;
  int8_t leftx, rightx;

  // Copy current snow to prevSnow
  memcpy(prevSnow, snow, sizeof(bool) * WIDTH * HEIGHT);

  // Move snow downwards
  for(y = 1; y != HEIGHT; y++) {
    for(x = 0; x != WIDTH; x++) {
      if(snow[x][y]) {
        // Figure next horizontal pixel to each side, wrapping if needed
        leftx = x - 1;
        if(leftx == -1) leftx = WIDTH - 1;
        rightx = x + 1;
        if(rightx == WIDTH) rightx = 0;
        if(!snow[x][y - 1]) {
          snow[x][y] = 0;
          snow[x][y - 1] = 1;
        } else if(x > 0 && !snow[leftx][y - 1]) {
          snow[x][y] = 0;
          snow[leftx][y - 1] = 1;
        } else if(x < (WIDTH - 1) && !snow[rightx][y - 1]) {
          snow[x][y] = 0;
          snow[rightx][y - 1] = 1;
        }
      }
    }
  }

  if(random(0, 100) < FLAKE_RATE) {
    // Seed a new snowflake
    snow[random(0, WIDTH)][HEIGHT - 1] = 1;
  }
}

void clearSnow()
{
  uint8_t x, y;

  for(x = 0; x != WIDTH; x++) {
    for(y = 0; y != HEIGHT; y++) {
      snow[x][y] = 0;
    }
  }
}

void drawSnow()
{
  uint8_t x, y;
  float frameF = frame;
  float progress = frameF / FADE_FRAMES;
  uint32_t color;
  uint8_t oldR, oldG, oldB, newR, newG, newB;

  if(progress > 1) progress = 1;

  for(x = 0; x != WIDTH; x++) {
    for(y = 0; y != HEIGHT; y++) {
      if(prevSnow[x][y]) {
        oldR = COLOR_SNOW_R;
        oldG = COLOR_SNOW_G;
        oldB = COLOR_SNOW_B;
      } else {
        oldR = COLOR_SKY_R;
        oldG = COLOR_SKY_G;
        oldB = COLOR_SKY_B;
      }
      if(snow[x][y]) {
        newR = COLOR_SNOW_R;
        newG = COLOR_SNOW_G;
        newB = COLOR_SNOW_B;
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

