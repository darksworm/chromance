#pragma once

namespace Grid {
enum class KnotColor {
  Red = 0, 
  White = 1, 
  Green = 2, 
  Blue = 3
};

struct Knot {
  KnotColor color;
  const int led_count;
};

enum class Direction {
  Incoming = 0, 
  Outgoing = 1, 
  NotConnected = 2
};

struct Connection {
  KnotColor color;
  int strip_num;
  Direction direction;
};

const Knot knots[4] = {
    { KnotColor::Red,    6 * 14 },
    { KnotColor::White, 11 * 14 },
    { KnotColor::Green, 12 * 14 },
    { KnotColor::Blue,  11 * 14 }
};

auto R = KnotColor::Red;
auto B = KnotColor::Blue;
auto W = KnotColor::White;
auto G = KnotColor::Green;

auto IN = Direction::Incoming;
auto OUT = Direction::Outgoing;

auto NC = Connection{{}, 0, Direction::NotConnected};

// from bottom up, left to right
const Connection nodes[25][6] = {
  // level 0
  { NC, NC, {W, 0, OUT}, NC, {B, 0, OUT}, NC }, // absoute bottom

  // level 1
  { NC, NC, {W, 1, OUT}, {W, 7, OUT}, {W, 6, IN}, {W, 0, IN} },
  { NC, {B, 0, IN}, {B, 6, IN}, {B, 7, OUT}, {B, 1, OUT}, NC },

  // level 2
  { NC, NC, NC, {W, 2, OUT}, NC, {W, 1, IN} },
  { NC, {W, 6, OUT}, NC, {W, 5, IN}, NC, {B, 6, OUT} },
  { NC, {B, 1, IN}, NC, {B, 2, OUT}, NC, NC },

  // level 3
  { {W, 7, IN}, NC, {W, 8, OUT}, NC, {B, 5, IN}, NC },
  { {B, 7, IN}, NC, {G, 5, OUT}, NC, {B, 8, OUT}, NC },

  // level 4
  { {W, 2, IN}, NC, {R, 4, IN}, {W, 9, OUT}, {W, 3, OUT}, {W, 8, IN} },
  { {W, 5, OUT}, {B, 5, OUT}, {W, 4, IN}, {G, 6, OUT}, {B, 4, IN}, {G, 5, IN}}, // absolute center
  { {B, 2, IN}, {B, 8, IN}, {B, 3, OUT}, {B, 9, OUT}, {G, 0, OUT}, NC },

  // level 5
  { NC, NC, NC, {R, 3, IN}, NC, {R, 4, OUT} },
  { NC, {W, 3, IN}, NC, {R, 0, OUT}, NC, {W, 4, OUT} },
  { NC, {B, 4, OUT}, NC, {G, 4, IN}, NC, {B, 3, IN} },
  { NC, {G, 0, IN}, NC, {G, 1, OUT}, NC , NC},

  // level 6
  { {W, 9, IN}, NC, {R, 5, OUT}, NC, {W, 10, OUT}, NC },
  { {G, 6, IN}, NC, {G, 10, IN}, NC, {G, 7, OUT}, NC },
  { {B, 9, IN}, NC, {G, 11, IN}, NC, {B, 10, OUT}, NC },

  // level 7
  { {R, 3, OUT}, NC, NC, NC, {R, 2, IN}, {R, 5, IN} },
  { {R, 0, IN}, {W, 10, IN}, {R, 1, OUT}, NC, {G, 9, IN}, {G, 10, OUT} },
  { {G, 4, OUT}, {G, 7, IN}, {G, 8, OUT}, NC, {G, 3, IN}, {G, 11, OUT} },
  { {G, 1, IN}, {B, 10, IN}, {G, 2, OUT}, NC, NC, NC },

  // level 8
  { NC, {R, 2, OUT}, NC, NC, NC, {R, 1, IN} },
  { NC, {G, 9, OUT}, NC, NC, NC, {G, 8, IN} }, // absolute center top
  { NC, {G, 3, OUT}, NC, NC, NC, {G, 2, IN} }
};
}
