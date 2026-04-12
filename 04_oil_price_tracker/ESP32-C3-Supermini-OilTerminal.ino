#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- SETTINGS ---
const char* ssid = "steth";
const char* password = "ilovecomputers";
const char* oilApiUrl = "https://api.oilpriceapi.com/v1/demo/prices";
float preWarPrice = 67.0; // αρχική τιμή πριν τον "πόλεμο"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float lastPrice = 67; //τελευταία τιμή
void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9); // Specific for C3 SuperMini SCL ασπρο 9, SDA grizo 8, mauro gnd-gnd, portokali vcc ->3.3V
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("OIL TERMINAL...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(oilApiUrl); // https://api.oilpriceapi.com/v1/demo/prices
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<2048> doc;  // αρκετά μεγάλο για όλο το JSON
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        float currentPrice = 0.0;

        // Πάρε array "prices"
        JsonArray prices = doc["data"]["prices"].as<JsonArray>();

        // Βρες το WTI_USD
        for (JsonObject priceItem : prices) {
          const char* code = priceItem["code"];
          if (strcmp(code, "BRENT_CRUDE_USD") == 0) {
            currentPrice = priceItem["price"];
            break;
          }
        }

        // --- Εμφάνιση στην οθόνη ---
        display.clearDisplay();

        // Header
        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("CRUDE OIL (BRENT)");
        display.drawLine(0, 12, 128, 12, WHITE);

        // Price
        display.setTextSize(3);
        display.setCursor(0, 20);
        display.print("$");
        display.print(currentPrice, 2); // 2 decimal

        // Trend arrow
        if (lastPrice > 0) {
          if (currentPrice > lastPrice) {
            // Up Triangle
            display.fillTriangle(114, 0, 109, 7, 119, 7, WHITE); // Up
          } else if (currentPrice < lastPrice) {
            // Down Triangle
            display.fillTriangle(114, 7, 109, 0, 119, 0, WHITE); // Down
          }
        }

        display.drawLine(0, 50, 128, 50, WHITE);

        display.setTextSize(1);
        display.setCursor(0, 57); // λίγο πιο κάτω από την κύρια τιμή
        display.print("PRE WAR PRICE: $");
        display.print(preWarPrice, 1);

        //debug
        Serial.print("Updated at ");
        Serial.print(millis()/1000); // χρόνος σε δευτερόλεπτα από reset
        Serial.print("s: Brent Price = $");
        Serial.println(currentPrice, 2);



        display.display();
        lastPrice = currentPrice;

      } else {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("HTTP GET failed, code: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(240000); // 4 λεπτά update
}
