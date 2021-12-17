#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <FastLED.h>
#include <ArduinoOTA.h>

const char* host = "esp32";
const char* ssid = "dank";
const char* password = "dank";

#define BRIGHTNESS  25
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define UPDATES_PER_SECOND 150

CRGB leds[4][12 * 14];

int led_counts[4] = {
  6 * 14,  11 * 14,  12 * 14,  11 * 14
};

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

const CRGBPalette16 dankPalette =
{
  CRGB(255, 0, 0),
  CRGB(255, 77, 0),
  CRGB(255, 103, 0),
  CRGB(255, 129, 0),
};

CRGB getDebugColor(int knot_number) {
  if (knot_number == 0)
    return CRGB::Red;

  if (knot_number == 1)
    return CRGB::White;

  if (knot_number == 2)
    return CRGB::Green;

  return CRGB::Blue;
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  //    uint8_t brightness = 255;
  //
  //    int first = position(2, 5, 1);
  //
  //    for( int i = 0; i < CURRENT_NUM_LEDS ; i++) {
  //        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
  //        colorIndex += 3;
  //    }
}

void setup(void) {
  setCpuFrequencyMhz(240);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.begin();

  delay( 250 ); // power-up safety delay

  FastLED.addLeds<LED_TYPE, 32, COLOR_ORDER>(leds[0], 6 * 14).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 33, COLOR_ORDER>(leds[1], 11 * 14).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 25, COLOR_ORDER>(leds[2], 12 * 14).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 26, COLOR_ORDER>(leds[3], 11 * 14).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness( BRIGHTNESS );

  FastLED.clear();
  FastLED.show();

  delay(1000);

  currentPalette = dankPalette;
  currentBlending = LINEARBLEND;
}

void turnOnStrip(int knot, int strip) {
  for (int i = 0; i < 14; i++) {
    leds[knot][strip * 14 + i] = getDebugColor(knot);
  }
}

//void turnOnLed(int knot, int strip, int index = 0) {
//  leds[knot][strip * 14 + index] = getDebugColor(knot);
//}

enum class Knot {
  Red = 0, White = 1, Green = 2, Blue = 3
};

enum class Direction {
  Incoming = 0, Outgoing = 1, NotConnected = 2
};

struct Connection {
  Knot knot;
  int strip_num;
  Direction direction;
};

Connection NOT_CONNECTED{{}, 0, Direction::NotConnected};

int turnOnLed(struct Connection connection, int index) {
  const int offset = (connection.direction == Direction::Incoming ? 13 : 0);
  const int led_num = connection.strip_num * 14 + (index * (connection.direction == Direction::Incoming ? -1 : 1)) + offset;
  leds[(int)connection.knot][led_num] = getDebugColor((int)connection.knot);
}

void loop(void) {
  //
  //  for (int i = 0; i < 4; i++) {
  //    for (int j = 0; j < led_counts[i]; j++) {
  //      leds[i][j] = getDebugColor(i);
  //    }
  //  }

  Connection center_node[6] = {
    {Knot::White, 5, Direction::Outgoing},
    {Knot::Blue, 5, Direction::Outgoing},
    {Knot::White, 4, Direction::Incoming},
    {Knot::Green, 6, Direction::Outgoing},
    {Knot::Blue, 4, Direction::Incoming},
    {Knot::Green, 5, Direction:: Incoming}
  };

  auto NC = NOT_CONNECTED;
  auto W = Knot::White;
  auto B = Knot::Blue;
  auto R = Knot::Red;
  auto G = Knot::Green;
  auto OUT = Direction::Outgoing;
  auto IN = Direction::Incoming;

  // from bottom up, left to right
  Connection nodes[][6] = {
    // level 0
    { NC, NC, {W, 0, OUT}, NC, {B, 0, OUT}, NC },

    // level 1
    { NC, NC, {W, 1, OUT}, {W, 7, OUT}, {W, 6, IN}, {W, 0, IN} },
    { NC, {B, 0, IN}, {B, 6, IN}, {B, 7, OUT}, {B, 1, OUT}, NC },

    // level 2
    { NC, NC, NC, {W, 2, OUT}, NC, {W, 1, IN} },
    { NC, {W, 6, OUT}, NC, {W, 5, IN}, NC, {B, 6, OUT} },
    { NC, {B, 1, IN}, NC, {B, 2, OUT}, NC, NC },

    // level 3
    { {W, 7, IN}, NC, {W, 8, OUT}, NC, {B, 5, IN}, NC },
    { {B, 7, IN}, NC, {G, 5, OUT}, NC, {B, 8, OUT}, NC },

    // level 4
    { {W, 2, IN}, NC, {R, 4, IN}, {W, 9, OUT}, {W, 3, OUT}, {W, 8, IN} },
    { {W, 5, OUT}, {B, 5, OUT}, {W, 4, IN}, {G, 6, OUT}, {B, 4, IN}, {G, 5, IN}},
    { {B, 2, IN}, {B, 8, IN}, {B, 3, OUT}, {B, 9, OUT}, {G, 0, OUT}, NC },

    // level 5
    { NC, NC, NC, {R, 3, IN}, NC, {R, 4, OUT} },
    { NC, {W, 3, IN}, NC, {R, 0, OUT}, NC, {W, 4, OUT} },
    { NC, {B, 4, OUT}, NC, {G, 4, IN}, NC, {B, 3, IN} },
    { NC, {G, 0, IN}, NC, {G, 1, OUT}, NC , NC},

    // level 6
    { {W, 9, IN}, NC, {R, 5, OUT}, NC, {W, 10, OUT}, NC },
    { {G, 6, IN}, NC, {G, 10, IN}, NC, {G, 7, OUT}, NC },
    { {B, 9, IN}, NC, {G, 11, IN}, NC, {B, 10, OUT}, NC },

    // level 7
    { {R, 3, OUT}, NC, NC, NC, {R, 2, IN}, {R, 5, IN} },
    { {R, 0, IN}, {W, 10, IN}, {R, 1, OUT}, NC, {G, 9, IN}, {G, 10, OUT} },
    { {G, 4, OUT}, {G, 7, IN}, {G, 8, OUT}, NC, {G, 3, IN}, {G, 11, OUT} },
    { {G, 1, IN}, {B, 10, IN}, {G, 2, OUT}, NC, NC, NC },

    // level 8
    { NC, {R, 2, OUT}, NC, NC, NC, {R, 1, IN} },
    { NC, {G, 9, OUT}, NC, NC, NC, {G, 8, IN} },
    { NC, {G, 3, OUT}, NC, NC, NC, {G, 2, IN} }
  };

  for ( int z = 0; z < 25; z++) {
    FastLED.clear();
    FastLED.show();
    FastLED.delay(150);


    for (int j = 0; j < 14; j++) {
      for (int i = 0; i < 6; i++) {
        auto dir = nodes[z][i];

        if (dir.direction != Direction::NotConnected) {
          turnOnLed(dir, j);
        }
      }

      ArduinoOTA.handle();

      FastLED.show();
      FastLED.delay(100);
    }

    ArduinoOTA.handle();
  }


  // static uint8_t startIndex = 0;
  // startIndex = startIndex + 1; /* motion speed */
  //FillLEDsFromPaletteColors(startIndex);
  //FastLED.delay(1000 / UPDATES_PER_SECOND);
}
