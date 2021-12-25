#pragma once
#include "leds.h"

namespace Colors {
    CRGB multiply(CRGB c, double multiplier) {
        if (multiplier >= 1) {
            return c;
        }

        return CRGB(
                floor(c.r * multiplier),
                floor(c.g * multiplier),
                floor(c.b * multiplier)
        );
    }
}
