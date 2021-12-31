#include "../animation.h"
#include "../leds.h"
#include "../animation_manipulators.h"
#include "sequence.h"

using namespace AnimationManipulators;

// COOL MR GREEN - LISTERINE COLOR SCHEME

namespace AnimationSequences {
    const int ACCENT_DELAY = 5;
    const int HEX_ANIM_PARTS = 8;
    const CRGB DOMINANT_COLOR = CRGB(0, 184, 60);
    const CRGB ACCENT_COLOR = CRGB(6, 135, 81);

    Anim hexagon {
        3, 0, CRGB::Black,
        new Move[7] {
            Move::TOP_RIGHT,
            Move::UP,
            Move::TOP_LEFT,
            Move::BOTTOM_LEFT,
            Move::DOWN,
            Move::BOTTOM_RIGHT,
            Move::END
        }
    };

    class Hexagons : public Sequence {
        private:
            Anim animations[8] {
                copyAnim(hexagon, 5, 4, DOMINANT_COLOR),
                copyAnim(hexagon, 3, 4, DOMINANT_COLOR),
                copyAnim(hexagon, 1, 4, DOMINANT_COLOR),

                delayAnimation(mirrorAnimation(copyAnim(hexagon, 2, 3, ACCENT_COLOR)), ACCENT_DELAY),
                delayAnimation(mirrorAnimation(copyAnim(hexagon, 4, 3, ACCENT_COLOR)), ACCENT_DELAY),

                copyAnim(hexagon, 2, 1, DOMINANT_COLOR),
                copyAnim(hexagon, 4, 1, DOMINANT_COLOR),

                delayAnimation(mirrorAnimation(copyAnim(hexagon, 3, 0, ACCENT_COLOR)), ACCENT_DELAY),
            };
            Animation::AnimationExecution execs[HEX_ANIM_PARTS];

        protected:
            void initialize() override {
                for (auto i = 0; i < HEX_ANIM_PARTS; i++) {
                    execs[i] = { new Animation::Progress, &Animation::fadeInFadeOutStep };
                }
            }

            void makeStep () override {
                for (int i = 0; i < HEX_ANIM_PARTS; ++i) {
                    Animation::step(&animations[i], &execs[i]);
                }
            }

            void doReset() override {
                for (int i = 0; i < HEX_ANIM_PARTS; ++i) {
                    delete execs[i].progress;
                    execs[i].progress = new Animation::Progress;
                }
            }
    };

}
