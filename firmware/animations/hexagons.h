#include "../animation.h"
#include "../leds.h"
#include "hsv2rgb.h"
#include "../animation_manipulators.h"
#include "sequence.h"

using namespace AnimationManipulators;

#define ACCENT_DELAY 5
#define ANIM_COUNT 8
// COOL MR GREEN - LISTERINE COLOR SCHEME
#define DOMINANT_COLOR CRGB(0, 184, 60)
#define ACCENT_COLOR CRGB(6, 135, 81)

namespace AnimationSequences {
    Anim hexagon {
        3, 0, CRGB::Red,
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
            Anim anims[ANIM_COUNT] = {
                copyAnim(hexagon, 5, 4, DOMINANT_COLOR),
                copyAnim(hexagon, 3, 4, DOMINANT_COLOR),
                copyAnim(hexagon, 1, 4, DOMINANT_COLOR),

                delayAnimation(mirrorAnimation(copyAnim(hexagon, 2, 3, ACCENT_COLOR)), ACCENT_DELAY),
                delayAnimation(mirrorAnimation(copyAnim(hexagon, 4, 3, ACCENT_COLOR)), ACCENT_DELAY),

                copyAnim(hexagon, 2, 1, DOMINANT_COLOR),
                copyAnim(hexagon, 4, 1, DOMINANT_COLOR),

                delayAnimation(mirrorAnimation(copyAnim(hexagon, 3, 0, ACCENT_COLOR)), ACCENT_DELAY),
            };
            Animation::AnimationExecution execs[ANIM_COUNT];
            bool init = false;

        protected:
            void initialize() override {
                for (auto i = 0; i < ANIM_COUNT; i++) {
                    execs[i] = { new Animation::Progress, &Animation::fadeInStep };
                }
            }

            void makeStep () override {
                for (int i = 0; i < ANIM_COUNT; ++i) {
                    Animation::step(&anims[i], &execs[i]);
                }
            }

        public:
            void reset() {
                for (int i = 0; i < ANIM_COUNT; ++i) {
                    delete execs[i].progress;
                    execs[i].progress = new Animation::Progress;
                }
            }
    };

}
