#pragma once
#include "animation.h"

using Animation::Move;
using Animation::fadeInFadeOutStep;
using Animation::Progress;
using Animation::step;
using Animation::AnimationExecution;
using Anim = Animation::Animation;

namespace AnimationManipulators {
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

    Move flipMove(Move original) {
        switch(original) {
            case Move::END:
            case Move::SKIP:
                return original;
            case Move::UP:
                return Move::DOWN;
            case Move::DOWN:
                return Move::UP;
            case Move::TOP_LEFT:
                return Move::BOTTOM_LEFT;
            case Move::TOP_RIGHT:
                return Move::BOTTOM_RIGHT;
            case Move::BOTTOM_LEFT:
                return Move::TOP_LEFT;
            case Move::BOTTOM_RIGHT:
                return Move::TOP_RIGHT;
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

    int copyFlippedMoves(Move* old, Move* copy, int size) {
        for (int i = 0; i < size; i++) {
            copy[i] = flipMove(old[i]);
        }
    }

    Anim copyAnim(Anim &original, int x = -1, int y = -1, CRGB color = CRGB::Black) {
        int size = findMoveArraySize(original.moves);
        Move *moves = new Move[size]();
        copyMoves(original.moves, moves, size);

        return Anim {
            x == -1 ? original.start_x : x,
            y == -1 ? original.start_y : y,
            color.r + color.g + color.b == 0 ? original.color : color,
            moves
        };
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

    Anim flipAnimation(Anim a) {
        int size = findMoveArraySize(a.moves);
        Move *moves = new Move[size]();
        copyFlippedMoves(a.moves, moves, size);
        delete a.moves;
        a.moves = moves;
        return a;
    }
}
