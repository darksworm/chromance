#pragma once
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "grid.h"
#include "colors.h"
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

namespace LEDs {
CRGB leds[4][MAX_LEDS_IN_KNOT];

void delay(int ms) {
    FastLED.delay(ms);
}

void show() {
    FastLED.show();
}

void clear() {
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < Grid::knots[k].led_count; i++) {
            leds[k][i] = CRGB::Black;
        }
    }
    FastLED.clear();
    show();
}

void darkenLeds(int percentage) {
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < Grid::knots[k].led_count; i++) {
            leds[k][i] = Colors::multiply(leds[k][i], ((double) 100 - percentage) / 100);
        }
    }
}

void darkenStep(CRGB color, int step) {
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < Grid::knots[k].led_count; i++) {
            auto led = leds[k][i];
            if (led.r + led.g + led.b == 0) continue;

            leds[k][i] = Colors::multiply(color, ((double) 100 - step) / 100);
        }
    }
}

void lightenStep(CRGB color, int step) {
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < Grid::knots[k].led_count; i++) {
            auto led = leds[k][i];
            if (led.r + led.g + led.b == 0) continue;

            leds[k][i] = Colors::multiply(color, ((double) 100 + step) / 100);
        }
    }
}

void brightenLeds(int percentage) {
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < Grid::knots[k].led_count; i++) {
            leds[k][i] = Colors::multiply(leds[k][i], ((double) 100 + percentage) / 100);
        }
    }
}

void setBrightness(int brightness) {
    FastLED.setBrightness(brightness);
}

void setup(int brightness) {
    clear();
    setBrightness(0);

    set_max_power_in_milliwatts(28000);

    FastLED.addLeds<LED_TYPE, 32, COLOR_ORDER>(leds[0], Grid::knots[0].led_count).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, 33, COLOR_ORDER>(leds[1], Grid::knots[1].led_count).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, 25, COLOR_ORDER>(leds[2], Grid::knots[2].led_count).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, 26, COLOR_ORDER>(leds[3], Grid::knots[3].led_count).setCorrection(TypicalLEDStrip);

    setBrightness(brightness);
}

int getPosition(struct Grid::Connection connection, int index) {
    const int offset = (connection.direction == Grid::Direction::Incoming ? 13 : 0);
    return connection.strip_num * LEDS_IN_STRIP + (index * (connection.direction == Grid::Direction::Incoming ? -1 : 1)) + offset;
}

int turnOn(struct Grid::Connection connection, int index, CRGB color) {
    const int led_num = getPosition(connection, index);
    leds[(int)connection.color][led_num] = color;
}
}

