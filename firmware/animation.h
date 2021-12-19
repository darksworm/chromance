#include <FastLED.h>
#include "grid.h"

extern int turnOnLed(struct Grid::Connection connection, int index, CRGB color);

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
  if (current_move == Move::UP) {
    animation->progress->y += 2;
  } else if (current_move == Move::DOWN) {
    animation->progress->y -= 2;
  } else if (current_move == Move::TOP_LEFT) {
    animation->progress->y += 1;
    animation->progress->x -= 1;
  } else if (current_move == Move::TOP_RIGHT) {
    animation->progress->y += 1;
    animation->progress->x += 1;
  } else if (current_move == Move::BOTTOM_LEFT) {
    animation->progress->y -= 1;
    animation->progress->x -= 1;
  } else if (current_move == Move::BOTTOM_RIGHT) {
    animation->progress->y -= 1;
    animation->progress->x += 1;
  }
}

Move& getMove(struct Animation *animation) {
  return animation->moves[animation->progress->move_index];
}
}
