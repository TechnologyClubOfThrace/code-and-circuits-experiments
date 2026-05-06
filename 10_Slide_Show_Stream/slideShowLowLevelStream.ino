#include <SPI.h>
#include <SD.h>
#include <TFT.h>  // Arduino LCD library

// Pin definitions
#define sd_cs 4
#define lcd_cs 10
#define dc    9
#define rst   8

TFT TFTscreen = TFT(lcd_cs, dc, rst);

int currentImage = 1;
char filename[13]; // 001.BMP, 002.BMP, ...

void setup() {
  Serial.begin(9600);
  while (!Serial);

  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);

  if (!SD.begin(sd_cs)) {
    Serial.println("SD init failed!");
    while (1);
  }

  Serial.println("Slideshow starting...");
}

void loop() {
  sprintf(filename, "%03d.BMP", currentImage);
  Serial.print("Streaming: "); Serial.println(filename);

  if (!streamBMP(filename)) {
    Serial.println("Failed to load image, restarting slideshow...");
    currentImage = 1;
    delay(500);
    return;
  }

  currentImage++;
  delay(500); // χρόνος ανά εικόνα
}

// -------------------------------
// Streaming BMP pixel-by-pixel
// -------------------------------
bool streamBMP(const char *fname) {
  File bmpFile = SD.open(fname);
  if (!bmpFile) return false;

  // Ανάγνωση BMP header
  if (bmpFile.read() != 'B' || bmpFile.read() != 'M') {
    bmpFile.close();
    return false; // δεν είναι BMP
  }

  bmpFile.seek(10);
  uint32_t pixelOffset = 0;
  pixelOffset = bmpFile.read(); pixelOffset |= bmpFile.read() << 8;
  pixelOffset |= bmpFile.read() << 16; pixelOffset |= bmpFile.read() << 24;

  bmpFile.seek(18);
  uint16_t w = bmpFile.read(); w |= bmpFile.read() << 8;
  w |= bmpFile.read() << 16; w |= bmpFile.read() << 24;
  uint16_t h = bmpFile.read(); h |= bmpFile.read() << 8;
  h |= bmpFile.read() << 16; h |= bmpFile.read() << 24;

  // 16-bit BMP
  bmpFile.seek(pixelOffset);

  for (int y = h - 1; y >= 0; y--) { // BMP αποθηκεύει ανάποδα
    for (int x = 0; x < w; x++) {
      uint8_t b = bmpFile.read();
      uint8_t g = bmpFile.read();
      uint8_t r = bmpFile.read();

      // Μετατροπή 24-bit σε 16-bit (565)
      uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
      TFTscreen.setAddrWindow(x, y, x, y);
      TFTscreen.pushColor(color);
    }
  }

  bmpFile.close();
  return true;
}