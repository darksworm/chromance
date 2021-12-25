#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "credentials.h"
#include "animation.h"
#include "animation_sequence.h"
#include "grid.h"

#ifndef WIFI_SSID
#pragma message "Please define WIFI_SSID"
exit(1);
#endif

#ifndef WIFI_PASS
#pragma message "Please define WIFI_PASS"
exit(1);
#endif

#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define MAX_LEDS_IN_STRIP 12 * LEDS_IN_STRIP

struct LedTransition {
  CRGB color = CRGB::Black;
  byte step = 0;
};

CRGB leds[4][MAX_LEDS_IN_STRIP];
LedTransition ledTargets[4][MAX_LEDS_IN_STRIP];

CRGB getDebugColor(int knot_number) {
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

  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < Grid::knots[k].led_count; i++) {
      ledTargets[k][i] = {};
    }
  }
}

int turnOnLed(struct Grid::Connection connection, int index, CRGB color) {
  const int offset = (connection.direction == Grid::Direction::Incoming ? 13 : 0);
  const int led_num = connection.strip_num * LEDS_IN_STRIP + (index * (connection.direction == Grid::Direction::Incoming ? -1 : 1)) + offset;
  leds[(int)connection.color][led_num] = color;
}

void clearLeds() {
  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < Grid::knots[k].led_count; i++) {
      leds[k][i] = CRGB::Black;
    }
  }
  FastLED.clear();
}

int getLedPosition(struct Grid::Connection connection, int index) {
  const int offset = (connection.direction == Grid::Direction::Incoming ? 13 : 0);
  return connection.strip_num * LEDS_IN_STRIP + (index * (connection.direction == Grid::Direction::Incoming ? -1 : 1)) + offset;
}

void transitionLed(struct Grid::Connection connection, int index, CRGB targetColor, int step) {
  const int led_num = getLedPosition(connection, index);
  const auto target = &(ledTargets[(int)connection.color][led_num]);
  if (target->step == 0) {
    target->color = targetColor;
    target->step = step;
  }
}

void setup(void) {
  setCpuFrequencyMhz(240);
  connectWifi();
  ArduinoOTA.begin();
  delay(250); // power-up safety delay
  setupLeds();
  delay(1000);
}

Animation::Animation side{
  3,
  0,
  CRGB::Red,
  new Animation::Move[5] {
    Animation::Move::TOP_LEFT,
    Animation::Move::TOP_LEFT,
    Animation::Move::UP,
    Animation::Move::SKIP,
    Animation::Move::END
  },
  NULL
};

Animation::Animation side2{
  3,
  0,
  CRGB::Red,
  new Animation::Move[5] {
    Animation::Move::TOP_RIGHT,
    Animation::Move::TOP_RIGHT,
    Animation::Move::UP,
    Animation::Move::SKIP,
    Animation::Move::END
  },
  NULL
};

Animation::Animation side3{
  3, 8,
  CRGB::Red,
  new Animation::Move[5] {
    Animation::Move::BOTTOM_RIGHT,
    Animation::Move::BOTTOM_RIGHT,
    Animation::Move::DOWN,
    Animation::Move::SKIP,
    Animation::Move::END
  },
  NULL
};

Animation::Animation side4{
  3, 8,
  CRGB::Red,
  new Animation::Move[5] {
    Animation::Move::BOTTOM_LEFT,
    Animation::Move::BOTTOM_LEFT,
    Animation::Move::DOWN,
    Animation::Move::SKIP,
    Animation::Move::END
  },
  NULL
};

Animation::Animation center1 {
  3,
  4,
  CRGB::Red,
  new Animation::Move[3] {
    Animation::Move::TOP_RIGHT,
    Animation::Move::BOTTOM_RIGHT,
    Animation::Move::END
  },
  NULL
};

Animation::Animation center2 {
  3,
  4,
  CRGB::Red,
  new Animation::Move[3] {
    Animation::Move::TOP_LEFT,
    Animation::Move::BOTTOM_LEFT,
    Animation::Move::END
  },
  NULL
};

Animation::Animation center3 {
  3,
  4,
  CRGB::Red,
  new Animation::Move[3] {
    Animation::Move::BOTTOM_LEFT,
    Animation::Move::TOP_LEFT,
    Animation::Move::END
  },
  NULL
};

Animation::Animation center4 {
  3,
  4,
  CRGB::Red,
  new Animation::Move[3] {
    Animation::Move::BOTTOM_RIGHT,
    Animation::Move::TOP_RIGHT,
    Animation::Move::END
  },
  NULL
};

Animation::Animation m1 {
  3, 6,
  CRGB::Red,
  new Animation::Move[3] {
    Animation::Move::SKIP,
    Animation::Move::TOP_LEFT,
    Animation::Move::END
  },
  NULL
};

Animation::Animation m2 {
  3, 6,
  CRGB::Red,
  new Animation::Move[3] {
    Animation::Move::SKIP,
    Animation::Move::TOP_RIGHT,
    Animation::Move::END
  },
  NULL
};

int c = 0;
void loop(void) {
  ArduinoOTA.handle();

  Animation::step(&side);
  Animation::step(&side2);
  Animation::step(&side3);
  Animation::step(&side4);


//  if ( !c || !(center1.progress->move_index == 0 && center1.progress->led_index == 0) ) {
//    Animation::step(&center1);
//    Animation::step(&center2);
//    Animation::step(&center3);
//    Animation::step(&center4);
//  }
//
//
//  if (!c || !(m1.progress->move_index == 0 && m1.progress->led_index == 0) ) {
//    Animation::step(&m1);
//    Animation::step(&m2);
//  }

  c = 1;

  FastLED.show();
  FastLED.delay(33);
}

void showDebugColors(void) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < Grid::knots[i].led_count; j++) {
      leds[i][j] = getDebugColor(i);
    }
  }
}
