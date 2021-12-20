#pragma once
#include "animation.h"

namespace AnimationSequence {
enum class Type {
  Parallel,
  Sequential
};

enum class LoopingPolicy {
  HaltUntilAllComplete,
  NeverStop
};

struct Sequence {
  Animation::Animation *animations;
  LoopingPolicy policy;
  Type type;
};
}

AnimationSequence::Sequence hexagonTop {
  new Animation::Animation[3] {
    {
      3, 0,
      CRGB::Blue,
      new Animation::Move[4] {
        Animation::Move::TOP_LEFT,
        Animation::Move::TOP_LEFT,
        Animation::Move::UP,
        Animation::Move::END
      }, NULL
    },
    {
      3, 0,
      CRGB::Blue,
      new Animation::Move[4] {
        Animation::Move::TOP_RIGHT,
        Animation::Move::TOP_RIGHT,
        Animation::Move::UP,
        Animation::Move::END
      }, NULL
    },
    {}
  },
  AnimationSequence::LoopingPolicy::NeverStop,
  AnimationSequence::Type::Parallel
};

AnimationSequence::Sequence hexagonBot {
  new Animation::Animation[3] {
    {
      3, 8,
      CRGB::Blue,
      new Animation::Move[4] {
        Animation::Move::BOTTOM_RIGHT,
        Animation::Move::BOTTOM_RIGHT,
        Animation::Move::DOWN,
        Animation::Move::END
      }, NULL
    },
    {
      3, 8,
      CRGB::Blue,
      new Animation::Move[4] {
        Animation::Move::BOTTOM_LEFT,
        Animation::Move::BOTTOM_LEFT,
        Animation::Move::DOWN,
        Animation::Move::END
      }, NULL
    },
    {}
  },
  AnimationSequence::LoopingPolicy::NeverStop,
  AnimationSequence::Type::Parallel
};
