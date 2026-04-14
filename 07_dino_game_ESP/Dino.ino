#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

/* =========================
   ideaspark / TTGO-variant pins (KEEP)
   ========================= */
#define LCD_MOSI 23
#define LCD_SCLK 18
#define LCD_CS   15
#define LCD_DC   2
#define LCD_RST  4
#define LCD_BLK  32

// One external button only:
#define BTN_ONE  27   // GPIO27 -> button -> GND (LOW when pressed)

Adafruit_ST7789 lcd = Adafruit_ST7789(LCD_CS, LCD_DC, LCD_RST);

/* =========================
   Display config (KEEP)
   ========================= */
static const int TFT_W_PORTRAIT = 135;
static const int TFT_H_PORTRAIT = 240;

static const int SCREEN_W = 240;   // landscape
static const int SCREEN_H = 135;

static const int ROT = 1;

/* =========================
   Layout
   ========================= */
static const int HUD_H    = 22;
static const int GROUND_Y = 110;

/* =========================
   Sprites (XBitmap, 1-bit)
   A more "T-Rex" look + 2 frames
   Size: 26x24
   ========================= */
static const int DINO_W = 24;
static const int DINO_H = 24;

// Frame 0
// 'Dino0', 24x24px
const unsigned char dino0 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x1f, 0xf8, 0x00, 0x1b, 0xf8, 0x00, 0x1f, 0xf8, 0x00, 
	0x1f, 0xf8, 0x00, 0x1f, 0xf8, 0x00, 0x1f, 0x00, 0x00, 0x1f, 0xe0, 0x40, 0x3e, 0x00, 0x40, 0xfe, 
	0x00, 0x61, 0xff, 0x80, 0x73, 0xfe, 0x80, 0x7f, 0xfe, 0x00, 0x7f, 0xfe, 0x00, 0x3f, 0xfe, 0x00, 
	0x1f, 0xfc, 0x00, 0x0f, 0xf8, 0x00, 0x07, 0xf0, 0x00, 0x03, 0xb0, 0x00, 0x03, 0x10, 0x00, 0x02, 
	0x10, 0x00, 0x03, 0x18, 0x00, 0x00, 0x00, 0x00
};



//Dino1
// 'Dino24', 24x24px
const unsigned char dino1 [] PROGMEM = {
	0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0x27, 0xff, 0xe0, 0x07, 0xff, 0xe0, 
	0x07, 0xff, 0xe0, 0x07, 0xff, 0xff, 0x07, 0xff, 0xf8, 0x07, 0xff, 0xff, 0x83, 0xfd, 0xff, 0x80, 
	0xfd, 0xfe, 0x00, 0x79, 0xfe, 0x80, 0x31, 0xff, 0x80, 0x01, 0xff, 0x80, 0x01, 0xff, 0x80, 0x03, 
	0xff, 0xc0, 0x07, 0xff, 0xe0, 0x0f, 0xff, 0xf0, 0x1f, 0xff, 0xf2, 0x3f, 0xff, 0xf7, 0x3f, 0xff, 
	0xf7, 0xbf, 0xff, 0xe7, 0x3f, 0xff, 0xff, 0xff
};




// Bird 22x12
static const int BIRD_W = 22;
static const int BIRD_H = 12;

const unsigned char birdBmp[] PROGMEM = {
  0x00,0x00,0x00, 0x01,0x80,0x00, 0x03,0xC0,0x00, 0x07,0xE0,0x00,
  0x0F,0xF0,0x00, 0x1F,0xF8,0x00, 0x3F,0xFC,0x00, 0x1F,0xF8,0x00,
  0x0F,0xF0,0x00, 0x07,0xE0,0x00, 0x03,0xC0,0x00, 0x01,0x80,0x00
};

/* =========================
   Obstacles
   ========================= */
static const int DINO_X = 26;

static const int CACTUS_W  = 10;
static const int CACTUS_H1 = 18;
static const int CACTUS_H2 = 28;

// IMPORTANT: minimum gap between obstacles
static const int MIN_GAP = 85;      // never closer than this
static const int SPAWN_MIN = 250;   // new obstacle spawns off-screen
static const int SPAWN_MAX = 380;

/* =========================
   Physics (easy/friendly)
   ========================= */
float dinoY = 0;
float vY    = 0;

const float GRAVITY = 0.72f;
const float JUMP_V  = -12.6f;

static const int DINO_TOP_MIN = HUD_H + 2;
static const int DINO_GROUND  = GROUND_Y - DINO_H;

/* =========================
   Game state
   ========================= */
enum class GameMode : uint8_t { StartScreen, Running, GameOver };
GameMode mode = GameMode::StartScreen;

unsigned long startMs = 0;
unsigned long lastFrameMs = 0;
const unsigned long FRAME_MS = 33;

int bestScore = 0;
int level = 1;
unsigned long nextLevelAt = 120;

float speedPx = 2.8f;
bool night = false;

// Obstacles (2 cacti but spaced by rule)
float cX1 = 260;
float cX2 = 360;
int cH1 = CACTUS_H2;
int cH2 = CACTUS_H1;

// Bird (optional)
bool birdActive = false;
float bX = 340;
int bY = 72;

// Input latch
bool latchBtn = false;

// Animation
int runFrame = 0;

// Dirty tracking
int prevDinoY = DINO_GROUND;
int prevC1X = -999, prevC2X = -999;
int prevBirdX = -999, prevBirdY = -999;
int prevScoreDrawn = -1, prevLevelDrawn = -1;

/* =========================
   Helpers
   ========================= */
static inline bool pressedLow(int pin) { return digitalRead(pin) == LOW; }

void setBacklight(bool on) {
  pinMode(LCD_BLK, OUTPUT);
  digitalWrite(LCD_BLK, on ? HIGH : LOW);
}

uint16_t bg() { return night ? ST77XX_BLACK : ST77XX_WHITE; }
uint16_t fg() { return night ? ST77XX_WHITE : ST77XX_BLACK; }

int scoreNow() {
  return (int)((millis() - startMs) / 120);
}

bool intersects(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {
  return (ax < bx + bw) && (ax + aw > bx) && (ay < by + bh) && (ay + ah > by);
}

void clearRect(int x, int y, int w, int h) {
  if (x < 0) { w += x; x = 0; }
  if (y < 0) { h += y; y = 0; }
  if (x + w > SCREEN_W) w = SCREEN_W - x;
  if (y + h > SCREEN_H) h = SCREEN_H - y;
  if (w <= 0 || h <= 0) return;
  lcd.fillRect(x, y, w, h, bg());
}

/* =========================
   Drawing
   ========================= */
void drawHUD(int score) {
  lcd.fillRect(0, 0, SCREEN_W, HUD_H, bg());
  lcd.setTextColor(fg(), bg());
  lcd.setTextSize(1);

  lcd.setCursor(6, 6);
  lcd.print("Score:");
  lcd.print(score);

  lcd.setCursor(92, 6);
  lcd.print("Best:");
  lcd.print(bestScore);

  lcd.setCursor(170, 6);
  lcd.print("Lv ");
  lcd.print(level);
}

void drawGroundBand() {
  lcd.fillRect(0, GROUND_Y - 3, SCREEN_W, 12, bg());
  lcd.drawLine(0, GROUND_Y, SCREEN_W, GROUND_Y, fg());

  int offset = (millis() / 40) % 20;
  for (int x = -offset; x < SCREEN_W; x += 20) {
    lcd.drawLine(x, GROUND_Y + 4, x + 6, GROUND_Y + 4, fg());
  }
}

void drawDino(int x, int y) {
  //const unsigned char* spr = (runFrame == 0) ? dino0 : dino1;
  const unsigned char* spr = dino0;
  lcd.drawBitmap(x, y, spr, DINO_W, DINO_H, ST77XX_GREEN, ST77XX_WHITE);
  //ST77XX_GREEN
  // simple eye
  //lcd.drawPixel(x + 18, y + 7, fg());
}

void drawCactus(int x, int h) {
  lcd.fillRect(x, GROUND_Y - h, CACTUS_W, h, fg());
  lcd.fillRect(x - 4, GROUND_Y - (h / 2), 6, 3, fg());
  lcd.fillRect(x + CACTUS_W - 2, GROUND_Y - (h / 3), 6, 3, fg());
}

void drawBird(int x, int y) {
  lcd.drawXBitmap(x, y, birdBmp, BIRD_W, BIRD_H, fg());
}

void drawStartScreen() {
  lcd.fillScreen(bg());
  lcd.setTextColor(fg(), bg());

  lcd.setTextSize(2);
  lcd.setCursor(52, 32);
  lcd.print("DINO");

  lcd.setTextSize(1);
  lcd.setCursor(34, 65);
  lcd.print("Press button to START");

  lcd.setCursor(46, 84);
  lcd.print("Same button = JUMP");

  lcd.setCursor(62, 104);
  lcd.print("GPIO27");
}

void drawGameOver(int score) {
  lcd.fillScreen(bg());
  lcd.setTextColor(fg(), bg());

  lcd.setTextSize(2);
  lcd.setCursor(52, 40);
  lcd.print("GAME OVER");

  lcd.setTextSize(1);
  lcd.setCursor(40, 74);
  lcd.print("Press button to restart");

  lcd.setCursor(70, 94);
  lcd.print("Score: ");
  lcd.print(score);
}

/* =========================
   Spawning with spacing rule
   ========================= */
int randomCactusH() {
  return (random(0, 2) == 0) ? CACTUS_H1 : CACTUS_H2;
}

float spawnXNotNear(float otherX) {
  // spawn off-screen and ensure distance to the other cactus
  // tries a few times, then forces a safe position
  for (int i = 0; i < 8; i++) {
    float x = random(SPAWN_MIN, SPAWN_MAX);
    if (abs((int)(x - otherX)) >= MIN_GAP) return x;
  }
  // force: put it at least MIN_GAP ahead
  return otherX + MIN_GAP + random(20, 60);
}

/* =========================
   Game reset/start
   ========================= */
void startNewRun() {
  mode = GameMode::Running;

  dinoY = DINO_GROUND;
  vY = 0;

  level = 1;
  nextLevelAt = 120;
  night = false;

  speedPx = 2.8f;
  startMs = millis();

  cX1 = 260;
  cH1 = randomCactusH();

  cX2 = cX1 + MIN_GAP + 40; // guaranteed spacing at start
  cH2 = randomCactusH();

  birdActive = false;
  bX = 340;
  bY = random(58, 86);

  prevDinoY = (int)dinoY;
  prevC1X = (int)cX1;
  prevC2X = (int)cX2;
  prevBirdX = -999;
  prevBirdY = -999;
  prevScoreDrawn = -1;
  prevLevelDrawn = -1;

  lcd.fillScreen(bg());
  drawHUD(0);
  drawGroundBand();
  drawDino(DINO_X, (int)dinoY);
  drawCactus((int)cX1, cH1);
  drawCactus((int)cX2, cH2);
}

/* =========================
   Setup / Loop
   ========================= */
void setup() {
  Serial.begin(115200);
  pinMode(BTN_ONE, INPUT_PULLUP);

  setBacklight(true);

  SPI.begin(LCD_SCLK, -1, LCD_MOSI, LCD_CS);
  lcd.init(TFT_W_PORTRAIT, TFT_H_PORTRAIT);
  lcd.setSPISpeed(40000000);
  lcd.setRotation(ROT);

  randomSeed(esp_random());

  drawStartScreen();
}

void loop() {
  // Button edge (one button for everything)
  bool btn = pressedLow(BTN_ONE);
  bool btnEdge = false;
  if (btn && !latchBtn) { latchBtn = true; btnEdge = true; }
  if (!btn) latchBtn = false;

  // Start / Restart with same button
  if (btnEdge) {
    if (mode == GameMode::StartScreen) {
      startNewRun();
      return;
    }
    if (mode == GameMode::GameOver) {
      startNewRun();
      return;
    }
    // Running: jump (handled below but we can trigger immediately)
    if (mode == GameMode::Running && dinoY >= (DINO_GROUND - 0.5f)) {
      vY = JUMP_V;
    }
  }

  if (mode != GameMode::Running) return;

  // Frame limiter
  unsigned long now = millis();
  if (now - lastFrameMs < FRAME_MS) return;
  lastFrameMs = now;

  // Physics
  vY += GRAVITY;
  dinoY += vY;

  if (dinoY < DINO_TOP_MIN) { dinoY = DINO_TOP_MIN; if (vY < 0) vY = 0; }
  if (dinoY > DINO_GROUND)  { dinoY = DINO_GROUND;  vY = 0; }

  // Animation when on ground
  if (dinoY >= DINO_GROUND - 0.5f) runFrame = (millis() / 120) % 2;
  else runFrame = 0;

  // Move cacti
  cX1 -= speedPx;
  cX2 -= speedPx;

  // Respawn with spacing rule
  if (cX1 < -CACTUS_W) {
    cX1 = spawnXNotNear(cX2);
    cH1 = randomCactusH();
  }
  if (cX2 < -CACTUS_W) {
    cX2 = spawnXNotNear(cX1);
    cH2 = randomCactusH();
  }

  // Bird spawn (after score)
  int sc = scoreNow();
  if (!birdActive && sc > 70 && random(0, 100) < 3) {
    birdActive = true;
    bX = random(260, 380);
    bY = random(58, 86);
  }
  if (birdActive) {
    bX -= (speedPx + 0.6f);
    if (bX < -BIRD_W) birdActive = false;
  }

  // Difficulty curve (gentle)
  speedPx = 2.8f + (sc / 70) * 0.18f;

  // Level + day/night
  if ((unsigned long)sc >= nextLevelAt) {
    level++;
    nextLevelAt += 120;
    if (level % 2 == 0) {
      night = !night;
      lcd.fillScreen(bg());
      prevScoreDrawn = -1;
      prevLevelDrawn = -1;
    }
  }

  // FAIR collisions (smaller hitbox)
  int dX = DINO_X;
  int dY = (int)dinoY;

  int dHitX = dX + 6;
  int dHitY = dY + 6;
  int dHitW = DINO_W - 12;
  int dHitH = DINO_H - 10;

  int c1X = (int)cX1;
  int c1Y = GROUND_Y - cH1;

  int c2X = (int)cX2;
  int c2Y = GROUND_Y - cH2;

  bool hit1 = intersects(dHitX, dHitY, dHitW, dHitH,
                         c1X + 2, c1Y + 4, CACTUS_W - 4, cH1 - 4);

  bool hit2 = intersects(dHitX, dHitY, dHitW, dHitH,
                         c2X + 2, c2Y + 4, CACTUS_W - 4, cH2 - 4);

  bool hitBird = false;
  if (birdActive) {
    int bbX = (int)bX;
    hitBird = intersects(dHitX, dHitY, dHitW, dHitH,
                         bbX + 2, bY + 2, BIRD_W - 4, BIRD_H - 4);
  }

  if (hit1 || hit2 || hitBird) {
    mode = GameMode::GameOver;
    bestScore = max(bestScore, sc);
    drawGameOver(sc);
    return;
  }

  // RENDER (dirty rects)
  clearRect(DINO_X, prevDinoY, DINO_W + 10, DINO_H + 2);

  clearRect(prevC1X - 6, GROUND_Y - 42, CACTUS_W + 18, 50);
  clearRect(prevC2X - 6, GROUND_Y - 42, CACTUS_W + 18, 50);

  if (prevBirdX != -999) clearRect(prevBirdX - 2, prevBirdY - 2, BIRD_W + 4, BIRD_H + 4);

  drawGroundBand();

  if (sc != prevScoreDrawn || level != prevLevelDrawn) {
    drawHUD(sc);
    prevScoreDrawn = sc;
    prevLevelDrawn = level;
  }

  drawCactus(c1X, cH1);
  drawCactus(c2X, cH2);
  if (birdActive) drawBird((int)bX, bY);

  drawDino(DINO_X, dY);

  prevDinoY = dY;
  prevC1X = c1X;
  prevC2X = c2X;

  if (birdActive) { prevBirdX = (int)bX; prevBirdY = bY; }
  else { prevBirdX = -999; prevBirdY = -999; }
}