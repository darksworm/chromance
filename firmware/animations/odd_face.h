#include "../animation.h"
#include "../leds.h"
#include "sequence.h"
#include "../animation_manipulators.h"

#define ANIM_PARTS 10
#define ANIM_COLOR CRGB::Orange

using namespace AnimationManipulators;

namespace AnimationSequences {
    Animation::Animation hex1 {
        3, 0,
        ANIM_COLOR,
        new Animation::Move[4] {
            Animation::Move::TOP_LEFT,
            Animation::Move::TOP_LEFT,
            Animation::Move::UP,
            Animation::Move::END
        },
    };

    Anim hex2 = mirrorAnimation(copyAnim(hex1));
    Anim hex3 = flipAnimation(copyAnim(hex1, 3, 8));
    Anim hex4 = mirrorAnimation(copyAnim(hex3, 3, 8));

    Animation::Animation center1 {
        3, 4, ANIM_COLOR,
        new Animation::Move[4] {
            Animation::Move::SKIP,
            Animation::Move::TOP_RIGHT,
            Animation::Move::BOTTOM_RIGHT,
            Animation::Move::END
        },
    };

    Anim center2 = mirrorAnimation(copyAnim(center1));
    Anim center3 = flipAnimation(copyAnim(center2));
    Anim center4 = mirrorAnimation(copyAnim(center3));

    Animation::Animation m1 {
        3, 6, ANIM_COLOR,
        new Animation::Move[4] {
            Animation::Move::SKIP,
            Animation::Move::SKIP,
            Animation::Move::TOP_LEFT,
            Animation::Move::END
        },
    };

    Anim m2 = mirrorAnimation(copyAnim(m1));

    class OddFace : public Sequence {
        private:
        Animation::Animation animations[ANIM_PARTS] {
            hex1, hex2, hex3, hex4,
            center1, center2, center3, center4,
            m1, m2
        };
        Animation::AnimationExecution exec[ANIM_PARTS];
        int brightness_counter = 0;
        int counter = 0;

        protected:
        void initialize() override {
            for (auto i = 0; i < ANIM_PARTS; i++) {
                exec[i] = { new Animation::Progress, &Animation::fadeInStep };
            }
        }

        void makeStep () override {
            if (counter++ > 0 && exec[0].progress->move_index == 0 && exec[0].progress->led_index == 0) {
                // do nada
            } else {
                for (int i = 0; i < ANIM_PARTS; i++) {
                    Animation::step(&animations[i], &exec[i]);
                }
            }
        }

        public:
        void reset () override {
            counter = 0;
            brightness_counter = 0;
            for(int i = 0; i < ANIM_PARTS; i ++) {
                delete exec[i].progress;
                exec[i].progress = new Animation::Progress;
            }
        }
    };
};
