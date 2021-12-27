#include "../animation.h"
#include "../leds.h"
#include "hsv2rgb.h"

using Animation::Move;
using Animation::fadeInFadeOutStep;
using Animation::Progress;
using Animation::step;
using Animation::AnimationExecution;
using Anim = Animation::Animation;

namespace HexagonsAnimation {
    int findMoveArraySize(Move* moves) {
        int ctr = 0;
        do {
        } while (moves[ctr++] != Move::END);
        return ctr;
    }

    Move mirrorMove(Move original) {
        switch(original) {
            case Move::UP:
            case Move::DOWN:
            case Move::END:
            case Move::SKIP:
                return original;
            case Move::TOP_LEFT:
                return Move::TOP_RIGHT;
            case Move::TOP_RIGHT:
                return Move::TOP_LEFT;
            case Move::BOTTOM_LEFT:
                return Move::BOTTOM_RIGHT;
            case Move::BOTTOM_RIGHT:
                return Move::BOTTOM_LEFT;
        }
    }

    int copyMoves(Move* old, Move* copy, int size, int offset = 0) {
        for (int i = 0; i < size; i++) {
            copy[i + offset] = old[i];
        }
    }

    int copyMirroredMoves(Move* old, Move* copy, int size) {
        for (int i = 0; i < size; i++) {
            copy[i] = mirrorMove(old[i]);
        }
    }

    Anim copyAnim(Anim &original, int x, int y, CRGB color) {
        int size = findMoveArraySize(original.moves);
        Move *moves = new Move[size]();
        copyMoves(original.moves, moves, size);
        return Anim {x, y, color, moves};
    }

    Anim delayAnimation(Anim a, byte count) {
        int size = findMoveArraySize(a.moves);
        Move *moves = new Move[size + count]();
        for (int i = 0; i < count; ++i) {
            moves[i] = Move::SKIP;
        }

        copyMoves(a.moves, moves, size, count);
        delete a.moves;
        a.moves = moves;
        return a;
    }

    Anim mirrorAnimation(Anim a) {
        int size = findMoveArraySize(a.moves);
        Move *moves = new Move[size]();
        copyMirroredMoves(a.moves, moves, size);
        delete a.moves;
        a.moves = moves;
        return a;
    }

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

    Anim bigHexagon {
        3, 0, CRGB::Yellow,
            new Move[19] {
                Move::TOP_LEFT,

            }
    };


#define ACCENT_DELAY 5
#define ANIM_COUNT 8
// COOL MR GREEN - LISTERINECOLOR SCHEME
#define DOMINANT_COLOR CRGB(0, 184, 60)
#define ACCENT_COLOR CRGB(6, 135, 81)

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

    AnimationExecution execs[ANIM_COUNT];
    bool init = false;

    void step() {
        for (int i = 0; i < ANIM_COUNT; ++i) {
            if(!init) {
                execs[i] = { new Progress, &fadeInFadeOutStep };
            }

            Animation::step(&anims[i], &execs[i]);
        }
        init = true;
    }
}
