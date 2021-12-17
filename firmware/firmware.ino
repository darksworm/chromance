#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "credentials.h"
#include "grid.h"

#ifndef WIFI_SSID
#pragma message "Please define WIFI_SSID"
exit(1);
#endif

#ifndef WIFI_PASS
#pragma message "Please define WIFI_PASS"
exit(1);
#endif

#define BRIGHTNESS  12
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 150

CRGB leds[4][12 * 14];

CRGB getDebugColor(int knot_number) {
  return CRGB::Blue;
  if (knot_number == 0)
    return CRGB::Red;

  if (knot_number == 1)
    return CRGB::White;

  if (knot_number == 2)
    return CRGB::Green;

  return CRGB::Blue;
}

void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(250);
  }
}

void setupLeds() {
  FastLED.addLeds<LED_TYPE, 32, COLOR_ORDER>(leds[0], Grid::knots[0].led_count).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 33, COLOR_ORDER>(leds[1], Grid::knots[1].led_count).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 25, COLOR_ORDER>(leds[2], Grid::knots[2].led_count).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, 26, COLOR_ORDER>(leds[3], Grid::knots[3].led_count).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness( BRIGHTNESS );

  FastLED.clear();
  FastLED.show();
}

void setup(void) {
  setCpuFrequencyMhz(240);
  connectWifi();
  ArduinoOTA.begin();
  delay(250); // power-up safety delay
  setupLeds();
  delay(1000);
}

int turnOnLed(struct Grid::Connection connection, int index) {
  const int offset = (connection.direction == Grid::Direction::Incoming ? 13 : 0);
  const int led_num = connection.strip_num * 14 + (index * (connection.direction == Grid::Direction::Incoming ? -1 : 1)) + offset;
  leds[(int)connection.color][led_num] = getDebugColor((int)connection.color);
}

void turnOnNode(struct Grid::Node node) {
  for (int j = 0; j < 14; j++) {
    for (int i = 0; i < 6; i++) {
      auto dir = node.connections[i];
      if (dir.direction != Grid::Direction::NotConnected) {
        turnOnLed(dir, j);
      }
    }

    ArduinoOTA.handle();

    FastLED.show();
    FastLED.delay(66);
  }
}

void loop(void) {  
  auto branch1 = Grid::levels[5].nodes[4];
  auto branch2 = Grid::levels[5].nodes[2];
  auto branch3 = Grid::levels[3].nodes[4];
  auto branch4 = Grid::levels[3].nodes[2];
  auto branch5 = Grid::levels[2].nodes[3];
  auto branch6 = Grid::levels[6].nodes[3];
  
  auto center_node = Grid::levels[4].nodes[3];
  turnOnNode(Grid::levels[4].nodes[3]);

  FastLED.delay(45);
  
  for (int j = 0; j < 14; j++) {
    for (int i = 0; i < 6; i++) {
      auto dir = branch1.connections[i];
      if (dir.direction != Grid::Direction::NotConnected) {
        turnOnLed(dir, j);
      }
      dir = branch2.connections[i];
      if (dir.direction != Grid::Direction::NotConnected) {
        turnOnLed(dir, j);
      }
      dir = branch3.connections[i];
      if (dir.direction != Grid::Direction::NotConnected) {
        turnOnLed(dir, j);
      }
      dir = branch4.connections[i];
      if (dir.direction != Grid::Direction::NotConnected) {
        turnOnLed(dir, j);
      }
      dir = branch5.connections[i];
      if (dir.direction != Grid::Direction::NotConnected) {
        turnOnLed(dir, j);
      }
      dir = branch6.connections[i];
      if (dir.direction != Grid::Direction::NotConnected) {
        turnOnLed(dir, j);
      }
    }

    ArduinoOTA.handle();

    FastLED.show();
    FastLED.delay(66);
  }

//  branch1 = Grid::levels[4].nodes[1];
//  branch2 = Grid::levels[4].nodes[5];
//   for (int j = 0; j < 14; j++) {
//    for (int i = 0; i < 6; i++) {
//      auto dir = branch1.connections[i];
//      if (dir.direction != Grid::Direction::NotConnected) {
//        turnOnLed(dir, j);
//      }
//      dir = branch2.connections[i];
//      if (dir.direction != Grid::Direction::NotConnected) {
//        turnOnLed(dir, j);
//      }
//    }
//
//    ArduinoOTA.handle();
//
//    FastLED.show();
//    FastLED.delay(100);
//  }

  FastLED.delay(300);

  FastLED.clear();
  FastLED.show();

//  for ( int z = 0; z < 25; z++) {
//    FastLED.clear();
//    FastLED.show();
//    FastLED.delay(150);
//
//    for (int j = 0; j < 14; j++) {
//      for (int i = 0; i < 6; i++) {
//        auto dir = Grid::nodes[z][i];
//
//        if (dir.direction != Grid::Direction::NotConnected) {
//          turnOnLed(dir, j);
//        }
//      }
//
//      ArduinoOTA.handle();
//
//      FastLED.show();
//      FastLED.delay(100);
//    }
//
//    ArduinoOTA.handle();
//  }
}

void showDebugColors(void) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < Grid::knots[i].led_count; j++) {
      leds[i][j] = getDebugColor(i);
    }
  }
}
