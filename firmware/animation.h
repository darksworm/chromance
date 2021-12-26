#pragma once
#include "grid.h"
#include "colors.h"
#include "leds.h"

namespace Animation {
    enum class Move {
        UP = 3,
        DOWN = 0,
        TOP_LEFT = 2,
        TOP_RIGHT = 4,
        BOTTOM_LEFT = 1,
        BOTTOM_RIGHT = 5,
        END = -1,
        SKIP = -2
    };

    struct Progress {
        int initiated = 0;
        int x = 0;
        int y = 0;
        int led_index = 0;
        int move_index = 0;
        int step = 0;
    };

    struct Animation {
        const int start_x;
        const int start_y;
        CRGB color;
        Move *moves;
    };

    struct AnimationExecution {
        Progress* progress;
        void (*step)(Animation*, Progress*);
    };

    void initiate(struct Animation *animation, struct Progress *progress);
    void reset(struct Animation *animation, struct Progress *progress);
    void makeMove(struct Animation *animation, struct Progress *progress);
    void turnOnCurrentLED(struct Animation *animation);
    void advanceToNextLED(struct Animation *animation, struct Progress *progress);
    void advanceToNextMove(struct Animation *animation, struct Progress *progress);
    Move& getMove(struct Animation *animation, struct Progress *progress);
    void movePositionToNextNode(struct Animation *animation, struct Progress *progress);
    void adjustPosition(int* y, int* x, Move m);
    void adjustPreviousLEDs(struct Animation *animation);
    void doThing(struct Animation *animation);

    void step(struct Animation *animation, struct AnimationExecution* exec) {
        initiate(animation, exec->progress);
        exec->step(animation, exec->progress);
    }

    void initiate(struct Animation *animation, struct Progress *progress) {
        if (progress->initiated == 0) {
            reset(animation, progress);
        }
    }

    Move invertMove(enum Move m) {
        int index = (int)m;
        int opposite_index = (index + 3) % 6;
        return (Move)opposite_index;
    }

    void reset(struct Animation *animation, struct Progress *progress) {
        progress->initiated = 1;
        progress->move_index = 0;
        progress->led_index = 0;
        progress->step = 0;
        progress->x = animation->start_x;
        progress->y = animation->start_y;
    }

    void fadeInFadeOutStep(struct Animation *animation, struct Progress *progress) {
        auto maxBaseColor = Colors::getTopColor(animation->color);
        auto interval = 1;
        int hsc = ceil((double)maxBaseColor / interval);
        int fsc = hsc * 2;

        CRGB current_color;
        int current_step = progress->step++;

        int move_index = progress->move_index;
        int led_index = progress->led_index;

        int x = progress->x;
        int y = progress->y;

        auto next_move = animation->moves[move_index + 1];
        if(next_move == Move::END && led_index == 13) {
            if (current_step == fsc + (move_index * 14 + led_index)) {
                reset(animation, progress);
                return;
            }
        } else {
            advanceToNextLED(animation, progress);
        }

        for (; move_index >= 0; --move_index) {
            for (; led_index >= 0; --led_index) {
                auto relative_step = current_step - (move_index * 14 + led_index) + 1;
                auto node = Grid::levels[y].nodes[x];
                auto strip = node.connections[(int)animation->moves[move_index]];
                auto led_descending = relative_step > hsc;

                if (animation->moves[move_index] == Move::SKIP) {
                    continue;
                }

                if(relative_step > fsc) {
                    current_color = CRGB(0, 0, 0);
                } else if (led_descending) {
                    int nc = maxBaseColor - ((relative_step - hsc) * interval);
                    nc = nc > 0 ? nc : 0;
                    current_color = Colors::multiply(animation->color, (double)nc / maxBaseColor);
                } else {
                    int nc = relative_step * interval;
                    current_color = Colors::multiply(animation->color, (double)nc / maxBaseColor);
                }

                LEDs::turnOn(strip, led_index, current_color);
            }

            if (move_index > 0 && animation->moves[move_index - 1] != Move::SKIP) {
                auto m = invertMove(animation->moves[move_index - 1]);
                adjustPosition(&y, &x, m);
            }

            led_index = 13;
        }
    }

    void fadeInStep(struct Animation *animation, struct Progress *progress) {
        auto maxBaseColor = Colors::getTopColor(animation->color);
        auto interval = 1;
        int fsc = ceil((double)maxBaseColor / interval);
        CRGB current_color;
        int current_step = progress->step++;

        int move_index = progress->move_index;
        int led_index = progress->led_index;

        int x = progress->x;
        int y = progress->y;

        auto next_move = animation->moves[move_index + 1];
        if(next_move == Move::END && led_index == 13) {
            if (current_step == fsc + (move_index * 14 + led_index)) {
                reset(animation, progress);
                return;
            }
        } else {
            advanceToNextLED(animation, progress);
        }

        for (; move_index >= 0; --move_index) {
            for (; led_index >= 0; --led_index) {
                auto relative_step = current_step - (move_index * 14 + led_index) + 1;
                auto node = Grid::levels[y].nodes[x];
                auto strip = node.connections[(int)animation->moves[move_index]];

                if (animation->moves[move_index] == Move::SKIP) {
                    continue;
                }

                if(relative_step > fsc) {
                    current_color = animation->color;
                } else {
                    int nc = relative_step * interval;
                    current_color = Colors::multiply(animation->color, (double)nc / maxBaseColor);
                }

                LEDs::turnOn(strip, led_index, current_color);
            }

            if (move_index > 0 && animation->moves[move_index - 1] != Move::SKIP) {
                auto m = invertMove(animation->moves[move_index - 1]);
                adjustPosition(&y, &x, m);
            }

            led_index = 13;
        }
    }

    void advanceToNextLED(struct Animation *animation, struct Progress *progress) {
        if (++progress->led_index == 14) {
            advanceToNextMove(animation, progress);
        }
    }

    void advanceToNextMove(struct Animation *animation, struct Progress *progress) {
        movePositionToNextNode(animation, progress);
        progress->led_index = 0;
        progress->move_index++;
    }

    void movePositionToNextNode(struct Animation *animation, struct Progress *progress) {
        auto current_move = getMove(animation, progress);
        adjustPosition(&progress->y, &progress->x, current_move);
    }

    void adjustPosition(int* y, int* x, Move m) {
        if (m == Move::UP) {
            (*y) += 2;
        } else if (m == Move::DOWN) {
            (*y) -= 2;
        } else if (m == Move::TOP_LEFT) {
            (*y) += 1;
            (*x) -= 1;
        } else if (m == Move::TOP_RIGHT) {
            (*y) += 1;
            (*x) += 1;
        } else if (m == Move::BOTTOM_LEFT) {
            (*y) -= 1;
            (*x) -= 1;
        } else if (m == Move::BOTTOM_RIGHT) {
            (*y) -= 1;
            (*x) += 1;
        }
    }

    Move& getMove(struct Animation *animation, struct Progress *progress) {
        return animation->moves[progress->move_index];
    }
}
