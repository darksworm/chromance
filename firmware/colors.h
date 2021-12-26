#pragma once
#include "leds.h"

namespace Colors {
    byte getTopColor(CRGB color) {
        auto max = color.r > color.g ? color.r : color.g;
        return max > color.b ? max : color.b;
    }

    byte max(int a, int b) {
        return a > b ? a : b;
    }

    byte min(int a, int b) {
        return a > b ? b : a;
    }

    CRGB multiply(CRGB c, double multiplier) {
        if(multiplier > 1) {
            return CRGB(
                min(255, floor(c.r * multiplier)),
                min(255, floor(c.g * multiplier)),
                min(255, floor(c.b * multiplier))
            );
        } else {
            return CRGB(
                max(0, floor(c.r * multiplier)),
                max(0, floor(c.g * multiplier)),
                max(0, floor(c.b * multiplier))
            );
        }
    }
}
