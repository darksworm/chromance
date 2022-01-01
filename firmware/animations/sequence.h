#pragma once
#include "../leds.h"

namespace AnimationSequences {
    const byte SequenceColorCount = 3;
    class Sequence {
        protected:
            virtual void makeStep() = 0;
            virtual void initialize() = 0;
            virtual void doReset() = 0;
            CRGB colors[SequenceColorCount];
        private:
            bool initialized = false;
        public:
            void reset() {
                if (initialized) {
                    doReset();
                }
            }
            void step() {
                if (!initialized) {
                    initialize();
                    initialized = true;
                }
                makeStep();
            }
            void setColor(int i, CRGB color) {
                if (i >= SequenceColorCount)
                    return;

                colors[i] = color;
            }
    };
}
