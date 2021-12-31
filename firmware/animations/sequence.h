#pragma once

namespace AnimationSequences {
    class Sequence {
        protected:
            virtual void makeStep() = 0;
            virtual void initialize() = 0;
            virtual void doReset() = 0;
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
    };
}
