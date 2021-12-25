#pragma once
#include <FastLED.h>
#include "grid.h"

extern CRGB leds[4][MAX_LEDS_IN_KNOT];
namespace Debug {
    CRGB getDebugColor(int knot_number) {
        if (knot_number == 0)
            return CRGB::Red;

        if (knot_number == 1)
            return CRGB::White;

        if (knot_number == 2)
            return CRGB::Green;

        return CRGB::Blue;
    }

    void showDebugColors(void) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < Grid::knots[i].led_count; j++) {
                leds[i][j] = getDebugColor(i);
            }
        }
    }
}
