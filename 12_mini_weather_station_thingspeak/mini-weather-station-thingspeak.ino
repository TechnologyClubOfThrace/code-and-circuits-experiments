#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SDA_PIN 4
#define SCL_PIN 5

// WiFi
const char* ssid = "xxx";
const char* password = "xxx";

// ThingSpeak
String apiKey = "xxx";

// BME280
Adafruit_BME280 bme;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
  delay(1000);

  // I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!bme.begin(0x76)) { // δοκίμασε 0x77 αν χρειαστεί
    Serial.println("Δεν βρέθηκε BME280!");
    while (1);
  }

  Serial.println("BME280 OK");

  // Διαβάζουμε αισθητήρα
  float temperature = bme.readTemperature();
  float humidity    = bme.readHumidity();
  float pressure    = bme.readPressure() / 100.0F;

  Serial.println("----- Measurements -----");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Σύνδεση WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 30) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("\nWiFi Connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    WiFiClient client;
    HTTPClient http;

    String url =
      "http://api.thingspeak.com/update?api_key=" + apiKey +
      "&field1=" + String(temperature, 2) +
      "&field2=" + String(humidity, 2) +
      "&field3=" + String(pressure, 2);

    Serial.println(url);

    http.begin(client, url);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String response = http.getString();
      Serial.print("ThingSpeak Entry ID: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

  } else {
    Serial.println("\nWiFi connection failed");
  }

  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Entering Deep Sleep...");
  delay(100);

  esp_sleep_enable_timer_wakeup(15ULL * 1000000ULL);
  esp_deep_sleep_start();
}

void loop() {
  // Δεν εκτελείται ποτέ λόγω deep sleep
}