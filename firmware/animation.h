#pragma once
#include <FastLED.h>
#include "grid.h"

extern int turnOnLed(struct Grid::Connection connection, int index, CRGB color);
void transitionLed(struct Grid::Connection connection, int index, CRGB targetColor, int amount);

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
  Progress *progress;
};

void initiate(struct Animation *animation);
void move(struct Animation *animation);
void reset(struct Animation *animation);
void makeMove(struct Animation *animation);
void turnOnCurrentLED(struct Animation *animation);
void advanceToNextLED(struct Animation *animation);
void advanceToNextMove(struct Animation *animation);
Move& getMove(struct Animation *animation);
void movePositionToNextNode(struct Animation *animation);
void adjustPosition(int* y, int* x, Move m);
void adjustPreviousLEDs(struct Animation *animation);
void doThing(struct Animation *animation);

void step(struct Animation *animation) {
  initiate(animation);
  move(animation);
}

void initiate(struct Animation *animation) {
  if (animation->progress == NULL) {
    animation->progress = new Progress;
    reset(animation);
  }
}

Move invertMove(enum Move m) {
   int index = (int)m;
   int opposite_index = (index + 3) % 6;
   return (Move)opposite_index;
}

void move(struct Animation *animation) {
  makeMove(animation);
}

void reset(struct Animation *animation) {
  animation->progress->move_index = 0;
  animation->progress->led_index = 0;
  animation->progress->step = 0;
  animation->progress->x = animation->start_x;
  animation->progress->y = animation->start_y;
}

void makeMove(struct Animation *animation) {
  doThing(animation);
}

void doThing(struct Animation *animation) {
  auto target_color = 200;
  auto interval = 5;
  int hsc = ceil((double)target_color / interval);
  int fsc = hsc * 2;

  CRGB current_color;
  int current_step = animation->progress->step++;

  int move_index = animation->progress->move_index;
  int led_index = animation->progress->led_index;

  int x = animation->progress->x;
  int y = animation->progress->y;

  auto next_move = animation->moves[move_index + 1];
  if(next_move == Move::END && led_index == 13) {
      if (current_step >= fsc + (move_index * 14 + led_index)) {
          reset(animation);
          return;
      }
  } else {
    advanceToNextLED(animation);
  }

  for (; move_index >= 0; --move_index) {
    for (; led_index >= 0; --led_index) {
       auto relative_step = current_step - (move_index * 14 + led_index) + 1;
       auto node = Grid::levels[y].nodes[x];
       auto strip = node.connections[(int)animation->moves[move_index]];
       auto led_descending = relative_step > hsc;

       if(relative_step > fsc) {
           current_color = CRGB(0,0,0);
       } else if (led_descending) {
         int nc = target_color - ((relative_step - hsc) * interval);
         current_color = CRGB(nc > 0 ? nc : 0, 0, 0);
       } else {
         current_color = CRGB(relative_step * interval, 0 ,0);
       }

       turnOnLed(strip, led_index, current_color);
    }

    if (move_index > 0 && animation->moves[move_index - 1] != Move::SKIP) {
      auto m = invertMove(animation->moves[move_index - 1]);
      adjustPosition(&y, &x, m);
    }

    led_index = 13;
  }
}

void advanceToNextLED(struct Animation *animation) {
  if (++animation->progress->led_index == 14) {
    advanceToNextMove(animation);
  }
}

void advanceToNextMove(struct Animation *animation) {
  movePositionToNextNode(animation);
  animation->progress->led_index = 0;
  animation->progress->move_index++;
}

void movePositionToNextNode(struct Animation *animation) {
  auto current_move = getMove(animation);
  adjustPosition(&animation->progress->y, &animation->progress->x, current_move);
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

Move& getMove(struct Animation *animation) {
  return animation->moves[animation->progress->move_index];
}
}
