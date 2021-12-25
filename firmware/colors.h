#pragma once
#include <FastLED.h>

namespace Colors {
    byte reduce(byte a, byte b) {
        return (a - b > 0) ? a - b : 0;
    }

    byte add(byte a, byte b) {
        return a + b < b || a + b < a ? 255 : a + b;
    }

    int minimum(int a, int b) {
        return a <= b ? a : b;
    }

    CRGB multiply(CRGB c, double multiplier) {
        return CRGB(
                floor(c.r * multiplier),
                floor(c.g * multiplier),
                floor(c.b * multiplier)
        );
    }

    CRGB stepColor(CRGB current, CRGB target, int amount) {
        CRGB step;

        if (current.r > target.r) {
            step.r = reduce(current.r, minimum(current.r - target.r, amount));
        }
        if (current.r < target.r) {
            step.r = add(current.r, minimum(target.r - current.r, amount));
        }

        if (current.g > target.g) {
            step.g = reduce(current.g, minimum(current.g - target.g, amount));
        }
        if (current.g < target.g) {
            step.g = add(current.g, minimum(target.g - current.g, amount));
        }

        if (current.b > target.b) {
            step.b = reduce(current.b, minimum(current.b - target.b, amount));
        }
        if (current.b < target.b) {
            step.b = add(current.b, minimum(target.b - current.b, amount));
        }

        return step;
    }
}
