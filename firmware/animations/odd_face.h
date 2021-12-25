#include "../animation.h"

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
            NULL
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
            NULL
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
            NULL
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
            NULL
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
            NULL
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
            NULL
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
            NULL
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
            NULL
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
            NULL
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
            NULL
    };

    void step() {
        Animation::step(&side);
        Animation::step(&side2);
        Animation::step(&side3);
        Animation::step(&side4);

        Animation::step(&center1);
        Animation::step(&center2);
        Animation::step(&center3);
        Animation::step(&center4);

        Animation::step(&m1);
        Animation::step(&m2);
    }
};
