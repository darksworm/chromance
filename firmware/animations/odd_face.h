#include "../animation.h"
#include "../leds.h"

namespace OddFace {
    auto animationColor = CRGB::Green;

    Animation::Animation side{
        3,
            0,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::TOP_LEFT,
                Animation::Move::TOP_LEFT,
                Animation::Move::UP,
                Animation::Move::END
            },
    };

    Animation::Animation side2{
        3,
            0,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::TOP_RIGHT,
                Animation::Move::TOP_RIGHT,
                Animation::Move::UP,
                Animation::Move::END
            },
    };

    Animation::Animation side3{
        3, 8,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::BOTTOM_RIGHT,
                Animation::Move::BOTTOM_RIGHT,
                Animation::Move::DOWN,
                Animation::Move::END
            },
    };

    Animation::Animation side4{
        3, 8,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::BOTTOM_LEFT,
                Animation::Move::BOTTOM_LEFT,
                Animation::Move::DOWN,
                Animation::Move::END
            },
    };

    Animation::Animation center1 {
        3,
            4,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::SKIP,
                Animation::Move::TOP_RIGHT,
                Animation::Move::BOTTOM_RIGHT,
                Animation::Move::END
            },
    };

    Animation::Animation center2 {
        3,
            4,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::SKIP,
                Animation::Move::TOP_LEFT,
                Animation::Move::BOTTOM_LEFT,
                Animation::Move::END
            },
    };

    Animation::Animation center3 {
        3,
            4,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::SKIP,
                Animation::Move::BOTTOM_LEFT,
                Animation::Move::TOP_LEFT,
                Animation::Move::END
            },
    };

    Animation::Animation center4 {
        3,
            4,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::SKIP,
                Animation::Move::BOTTOM_RIGHT,
                Animation::Move::TOP_RIGHT,
                Animation::Move::END
            },
    };

    Animation::Animation m1 {
        3, 6,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::SKIP,
                Animation::Move::SKIP,
                Animation::Move::TOP_LEFT,
                Animation::Move::END
            },
    };

    Animation::Animation m2 {
        3, 6,
            animationColor,
            new Animation::Move[4] {
                Animation::Move::SKIP,
                Animation::Move::SKIP,
                Animation::Move::TOP_RIGHT,
                Animation::Move::END
            },
    };

    Animation::AnimationExecution exec[10] = {
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
        {new Animation::Progress, &Animation::fadeInStep},
    };

    int counter = 0;
    int brightness_counter = 0;
    bool lock = false;
    void step() {
        if (counter > 0 && exec[0].progress->move_index == 0 && exec[0].progress->led_index == 0) {
            // do nada
            //if(brightness_counter++ > 30) {
            //    LEDs::brightenLeds(1);

            //    if(brightness_counter == 60) {
            //        brightness_counter = 0;
            //    }
            //} else {
            //    LEDs::darkenLeds(1);
            //}
        } else {
            counter++;

            Animation::step(&side, &exec[0]);
            Animation::step(&side2, &exec[1]);
            Animation::step(&side3, &exec[2]);
            Animation::step(&side4, &exec[3]);

            Animation::step(&center1, &exec[4]);
            Animation::step(&center2, &exec[5]);
            Animation::step(&center3, &exec[6]);
            Animation::step(&center4, &exec[7]);

            Animation::step(&m1, &exec[8]);
            Animation::step(&m2, &exec[9]);
        }

    }
};
