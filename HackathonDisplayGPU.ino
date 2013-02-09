// Renders 512 colors on a 16x32 RGB LED matrix.
// Library supports 4096 colors, but there aren't that many pixels!

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Wire.h>
const byte MY_ADDRESS = 42;

#define CLK 8  // MUST be on PORTB!
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

void setup() {
  matrix.begin();
  Wire.begin (MY_ADDRESS);
  for (byte i = 2; i <= 7; i++)
    pinMode (i, OUTPUT);
  Wire.onReceive (receiveEvent);
}

void loop() {
  // do nothing
}

// called by interrupt service routine when incoming data arrives
void receiveEvent (int howMany)
{
  int c;
  c = Wire.read();
  if(c == 'p' && howMany == 5) { // pixel
      int x, y, cb1, cb2, co;
      x = Wire.read();
      y = Wire.read();
      cb1 = Wire.read(); // high color byte
      cb2 = Wire.read(); // low color byte
      co = cb1 << 8 | cb2;
      matrix.drawPixel(x, y, co);
  }
  else if(c == 'C' && howMany == 6) { // circle
      int x, y, r, cb1, cb2, co;
      x = Wire.read();
      y = Wire.read();
      r = Wire.read();
      cb1 = Wire.read(); // high color byte
      cb2 = Wire.read(); // low color byte
      co = cb1 << 8 | cb2;
      matrix.drawCircle(x, y, r, co);
  }
  else if(c == 't' && howMany == 9) { // character
    int x, y, ch, cb1, cb2, co, bg, size;
    x = Wire.read();
    y = Wire.read();
    ch = Wire.read();
    cb1 = Wire.read(); // high color byte
    cb2 = Wire.read(); // low color byte
    co = cb1 << 8 | cb2;
    cb1 = Wire.read(); // high color byte
    cb2 = Wire.read(); // low color byte
    bg = cb1 << 8 | cb2;
    size = Wire.read();
    matrix.drawChar(x, y, ch, co, bg, size);
  }
  else if(c == 'c' && howMany == 1) { // cls
    matrix.fillScreen(matrix.Color333(0,0,0));
  }
}
