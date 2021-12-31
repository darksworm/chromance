#pragma once

namespace AnimationSequences {
    class Sequence {
        protected:
            virtual void makeStep() = 0;
            virtual void initialize() = 0;
        private:
            bool initialized = false;
        public:
            virtual void reset() = 0;
            void step() {
                if (!initialized) {
                    initialize();
                    initialized = true;
                }
                makeStep();
            }
    };
}
