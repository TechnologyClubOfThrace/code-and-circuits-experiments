#include <Servo.h>

/* =========================
   Hardware & motion tuning
   ========================= */
// Pins
const int ServoXPin = 8;
const int ServoYPin = 9;
const int XAxlePin  = A0;
const int YAxlePin  = A1;

// Servo limits (degrees)
const int ServoXHomePos = 130;
const int ServoYHomePos = 70;
const int TravelDeg     = 20;   // ± travel around home (same with your original ±20)

// Update timing (non-blocking)
const uint16_t updateIntervalMs = 15; // ~66 Hz update rate

// Motion profile
const float maxSpeedDegPerSec = 180.0;   // max slew rate
const float easingFactor      = 0.35;    // extra easing near target (0..1), 0 = linear

// Joystick processing
const int   samplesToAvg   = 15;     // analog averaging per read
int         xCenter        = 512;   // will be auto-calibrated at startup
int         yCenter        = 512;   // will be auto-calibrated at startup
const int   deadzone       = 60;    // raw ADC deadzone around center (reduce jitter)
const float expo           = 1.6f;  // response curve (>1 = softer around center)
const float lpfAlpha       = 0.25f; // low-pass filter for target (0..1)

// Optional: detach servo when idle (reduces buzzing, not for load-bearing)
const bool  detachWhenIdle = false;
const uint32_t idleDetachMs = 2000;

/* =========================
   Globals
   ========================= */
Servo myServoX, myServoY;

float curX = ServoXHomePos, curY = ServoYHomePos;   // current commanded position
float tgtX = ServoXHomePos, tgtY = ServoYHomePos;   // target position
float filtTgtX = ServoXHomePos, filtTgtY = ServoYHomePos;

unsigned long lastUpdateMs = 0;
unsigned long lastMotionMs = 0;
bool xAttached = false, yAttached = false;

/* =========================
   Helpers
   ========================= */

// Read analog with simple averaging
int analogReadAvg(uint8_t pin, int samples) {
  long sum = 0;
  for (int i = 0; i < samples; ++i) sum += analogRead(pin);
  return (int)(sum / samples);
}

// Map joystick raw value to normalized -1..+1 with deadzone and expo
float joystickToNorm(int raw, int center) {
  int v = raw - center;
  if (abs(v) < deadzone) return 0.0f;

  // Normalize to -1..+1 considering full-scale after deadzone
  float span = (v > 0) ? (1023 - center - deadzone) : (center - deadzone);
  if (span <= 0) return 0.0f;
  float n = (float)(v > 0 ? v - deadzone : v + deadzone) / span;
  n = constrain(n, -1.0f, 1.0f);

  // Exponential response (preserve sign)
  float sign = (n >= 0) ? 1.0f : -1.0f;
  n = sign * powf(fabs(n), expo);
  return n;
}

// Low-pass filter
float lpf(float prev, float in, float alpha) {
  return prev + alpha * (in - prev);
}

// Attach if needed
void ensureAttached() {
  if (!xAttached) { myServoX.attach(ServoXPin); xAttached = true; }
  if (!yAttached) { myServoY.attach(ServoYPin); yAttached = true; }
}

// Optionally detach when idle
void maybeDetachIfIdle() {
  if (!detachWhenIdle) return;
  unsigned long now = millis();
  if (now - lastMotionMs >= idleDetachMs) {
    if (xAttached) { myServoX.detach(); xAttached = false; }
    if (yAttached) { myServoY.detach(); yAttached = false; }
  }
}

/* =========================
   Setup & loop
   ========================= */
void setup() {
  Serial.begin(9600);

  // Auto-calibrate joystick centers (simple average on startup)
  delay(50);
  xCenter = analogReadAvg(XAxlePin, 50);
  yCenter = analogReadAvg(YAxlePin, 50);

  // Initial attach and home
  myServoX.attach(ServoXPin); xAttached = true;
  myServoY.attach(ServoYPin); yAttached = true;

  curX = tgtX = filtTgtX = ServoXHomePos;
  curY = tgtY = filtTgtY = ServoYHomePos;

  myServoX.write((int)curX);
  myServoY.write((int)curY);

  lastUpdateMs = millis();
  lastMotionMs = lastUpdateMs;

  Serial.print("Centers AAAAAAAA -> X: "); Serial.print(xCenter);
  Serial.print("  Y: "); Serial.println(yCenter);
}

int prev_x = -1;
int prev_y = -1;

void loop() {
  // --- 1) Read & process joystick into target angles ---
  int rawX = analogReadAvg(XAxlePin, samplesToAvg);
  int rawY = analogReadAvg(YAxlePin, samplesToAvg);

  float nx = joystickToNorm(rawX, xCenter); // -1..+1
  float ny = joystickToNorm(rawY, yCenter); // -1..+1

  // Target around home within ±TravelDeg (apply soft expo already inside)
  tgtX = ServoXHomePos + nx * TravelDeg;
  tgtY = ServoYHomePos + ny * TravelDeg;

  // Optional low-pass on target (smooth sudden joystick spikes)
  filtTgtX = lpf(filtTgtX, tgtX, lpfAlpha);
  filtTgtY = lpf(filtTgtY, tgtY, lpfAlpha);

  // --- 2) Timed motion update (non-blocking) ---
  unsigned long now = millis();
  if (now - lastUpdateMs >= updateIntervalMs) {
    float dt = (now - lastUpdateMs) / 1000.0f;
    lastUpdateMs = now;

    // Slew-rate limit
    float maxStep = maxSpeedDegPerSec * dt; // degrees we allow this tick

    // Add easing: near target reduce step proportionally
    float dx = filtTgtX - curX;
    float dy = filtTgtY - curY;

    float stepX = constrain(fabs(dx) * easingFactor, 0.5f, maxStep);
    float stepY = constrain(fabs(dy) * easingFactor, 0.5f, maxStep);

    if (fabs(dx) <= stepX) curX = filtTgtX;
    else                  curX += (dx > 0 ? stepX : -stepX);

    if (fabs(dy) <= stepY) curY = filtTgtY;
    else                  curY += (dy > 0 ? stepY : -stepY);

    // Hard constraints to 0..180 (and within our travel around home)
    curX = constrain(curX, ServoXHomePos - TravelDeg, ServoXHomePos + TravelDeg);
    curY = constrain(curY, ServoYHomePos - TravelDeg, ServoYHomePos + TravelDeg);
    curX = constrain(curX, 0, 180);
    curY = constrain(curY, 0, 180);

    if ((int)roundf(curX) != prev_x || (int)roundf(curY) != prev_y ) {
      prev_x = (int)roundf(curX);
      prev_y = (int)roundf(curY);

      // Attach if we had detached; write outputs
      ensureAttached();

      Serial.println(prev_x);
      Serial.println(prev_y);

      myServoX.write(prev_x);
      myServoY.write(prev_y);
    }

    // Motion activity timestamp
    if (fabs(dx) > 0.1f || fabs(dy) > 0.1f) lastMotionMs = now;

    // Optional debug
    // Serial.print("rawX:"); Serial.print(rawX);
    // Serial.print(" rawY:"); Serial.print(rawY);
    // Serial.print(" tgtX:"); Serial.print(tgtX,1);
    // Serial.print(" curX:"); Serial.print(curX,1);
    // Serial.print("  |  tgtY:"); Serial.print(tgtY,1);
    // Serial.print(" curY:"); Serial.println(curY,1);
  }

  // --- 3) Optional power saving / buzzing reduction ---
  maybeDetachIfIdle();
}
