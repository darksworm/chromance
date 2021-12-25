#include "../animation.h"
#include "../grid.h"

namespace DebugAnimation {
    Animation::Animation top {
        6, 7,
        CRGB::Red,
        new Animation::Move[7] {
            Animation::Move::BOTTOM_LEFT,
            Animation::Move::TOP_LEFT,

            Animation::Move::BOTTOM_LEFT,
            Animation::Move::TOP_LEFT,

            Animation::Move::BOTTOM_LEFT,
            Animation::Move::TOP_LEFT,

            Animation::Move::END
        },
        NULL
    };

    Animation::Animation middle_top {
        5, 4,
        CRGB::Red,
        new Animation::Move[5] {
            Animation::Move::TOP_LEFT,
            Animation::Move::BOTTOM_LEFT,

            Animation::Move::TOP_LEFT,
            Animation::Move::BOTTOM_LEFT,

            Animation::Move::END
        }
    };

    Animation::Animation middle_bottom {
        5, 4,
        CRGB::Red,
        new Animation::Move[5] {
            Animation::Move::BOTTOM_LEFT,
            Animation::Move::TOP_LEFT,

            Animation::Move::BOTTOM_LEFT,
            Animation::Move::TOP_LEFT,

            Animation::Move::END
        }
    };

    Animation::Animation bottom {
        4, 1,
        CRGB::Red,
        new Animation::Move[3] {
            Animation::Move::TOP_LEFT,
            Animation::Move::BOTTOM_LEFT,

            Animation::Move::END
        }
    };

    Animation::Animation outside_r {
        3, 0,
        CRGB::Red,
        new Animation::Move[9] {
            Animation::Move::TOP_LEFT,
            Animation::Move::TOP_LEFT,
            Animation::Move::UP,
            Animation::Move::TOP_LEFT,
            Animation::Move::UP,
            Animation::Move::TOP_RIGHT,
            Animation::Move::BOTTOM_RIGHT,
            Animation::Move::TOP_RIGHT,
            Animation::Move::END
        },
        NULL
    };

    Animation::Animation outside_l {
        3, 0,
        CRGB::Red,
        new Animation::Move[9] {
            Animation::Move::TOP_RIGHT,
            Animation::Move::TOP_RIGHT,
            Animation::Move::UP,
            Animation::Move::TOP_RIGHT,
            Animation::Move::UP,
            Animation::Move::TOP_LEFT,
            Animation::Move::BOTTOM_LEFT,
            Animation::Move::TOP_LEFT,
            Animation::Move::END
        },
        NULL
    };


    void step() {
        Animation::step(&outside_r);
        Animation::step(&outside_l);
        Animation::step(&top);
        Animation::step(&middle_top);
        Animation::step(&middle_bottom);
        Animation::step(&bottom);
    }
}

