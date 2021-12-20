#pragma once
#include <FastLED.h>
#include "grid.h"

extern int turnOnLed(struct Grid::Connection connection, int index, CRGB color);
extern int fadeLed(struct Grid::Connection connection, int index, int amount);

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

Move invertMove(enum class Move m) {
   int index = (int)m;
   int opposite_index = (index + 3) % 6;
   return (Move)opposite_index;
}

void fadeStep(struct Animation *animation) {
  int x = animation->progress->x;
  int y = animation->progress->y;

  int move_index = animation->progress->move_index;
  int led_index = animation->progress->led_index - 1;

  for (; move_index >= 0; --move_index) {
    for (; led_index >= 0; --led_index) {
       auto node = Grid::levels[y].nodes[x];
       auto strip = node.connections[(int)animation->moves[move_index]];
       fadeLed(strip, led_index, 30);
    }
    if (animation->moves[move_index - 1] != Move::SKIP) {
      auto m = invertMove(animation->moves[move_index - 1]);
      adjustPosition(&y, &x, m);
    }
    led_index = 13;
  }  
}

void move(struct Animation *animation) {
  if (getMove(animation) == Move::END) {
    reset(animation);
  } else {
    makeMove(animation);
  }
}

void reset(struct Animation *animation) {
  animation->progress->move_index = 0;
  animation->progress->led_index = 0;
  animation->progress->x = animation->start_x;
  animation->progress->y = animation->start_y;
}

void makeMove(struct Animation *animation) {
  turnOnCurrentLED(animation);
  advanceToNextLED(animation);
}

void turnOnCurrentLED(struct Animation *animation) {
  if (getMove(animation) != Move::SKIP) {
    auto node = Grid::levels[animation->progress->y].nodes[animation->progress->x];
    auto strip = node.connections[(int)getMove(animation)];
    turnOnLed(strip, animation->progress->led_index, animation->color);
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
