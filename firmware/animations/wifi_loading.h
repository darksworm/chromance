#pragma once
#include "../animation.h";

namespace WifiLoading {
    Animation::Animation wifiLoading{
        3,
        4,
        CRGB::Blue,
        new Animation::Move[7] {
            Animation::Move::TOP_LEFT,
            Animation::Move::UP,
            Animation::Move::TOP_RIGHT,
            Animation::Move::BOTTOM_RIGHT,
            Animation::Move::DOWN,
            Animation::Move::BOTTOM_LEFT,
            Animation::Move::END
        },
    };

    Animation::AnimationExecution exec {
        new Animation::Progress(), &Animation::fadeInFadeOutStep
    };

    void step() {
        Animation::step(&wifiLoading, &exec);
    }
}
