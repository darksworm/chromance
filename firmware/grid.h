#pragma once

#define LEDS_IN_STRIP 14
#define FIRST_KNOT_LEDS    6 * LEDS_IN_STRIP
#define SECOND_KNOT_LEDS  11 * LEDS_IN_STRIP
#define THIRD_KNOT_LEDS   12 * LEDS_IN_STRIP
#define FOURTH_KNOT_LEDS  11 * LEDS_IN_STRIP
#define TOTAL_LEDS FIRST_KNOT_LEDS + SECOND_KNOT_LEDS + THIRD_KNOT_LEDS + FOURTH_KNOT_LEDS

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
        { KnotColor::Red,   FIRST_KNOT_LEDS },
        { KnotColor::White, SECOND_KNOT_LEDS },
        { KnotColor::Green, THIRD_KNOT_LEDS },
        { KnotColor::Blue,  FOURTH_KNOT_LEDS }
    };

    auto R = KnotColor::Red;
    auto B = KnotColor::Blue;
    auto W = KnotColor::White;
    auto G = KnotColor::Green;

    auto IN = Direction::Incoming;
    auto OUT = Direction::Outgoing;

    auto NC = Connection{{}, 0, Direction::NotConnected};

    struct Node {
        Connection connections[6];
    };

    struct Level {
        Node nodes[7];
    };

    auto NULL_NODE = Node{ NC, NC, NC, NC, NC, NC };

    // from bottom up, left to right
    const Level levels[9] = {
        {{
             // level 0
             NULL_NODE,
             NULL_NODE,
             NULL_NODE,
             { { NC, NC, {W, 0, OUT}, NC, {B, 0, OUT}, NC } }, // absolute bottom
             NULL_NODE,
             NULL_NODE,
             NULL_NODE
         }},

        {{
             // level 1
             NULL_NODE,
             NULL_NODE,
             {{ NC, NC, {W, 1, OUT}, {W, 7, OUT}, {W, 6, IN}, {W, 0, IN} }},
             NULL_NODE,
             {{ NC, {B, 0, IN}, {B, 6, IN}, {B, 7, OUT}, {B, 1, OUT}, NC }},
             NULL_NODE,
             NULL_NODE
         }},

        {{
             // level 2
             NULL_NODE,
             { { NC, NC, NC, {W, 2, OUT}, NC, {W, 1, IN} }},
             NULL_NODE,
             { { NC, {W, 6, OUT}, NC, {W, 5, IN}, NC, {B, 6, OUT} } },
             NULL_NODE,
             { { NC, {B, 1, IN}, NC, {B, 2, OUT}, NC, NC } } ,
             NULL_NODE
         }},

        {{
             // level 3
             NULL_NODE,
             NULL_NODE,
             { { {W, 7, IN}, NC, {W, 8, OUT}, NC, {B, 5, IN}, NC }} ,
             NULL_NODE,
             { { {B, 7, IN}, NC, {G, 5, OUT}, NC, {B, 8, OUT}, NC } },
             NULL_NODE,
             NULL_NODE,
         }},

        {{
             // level 4
             NULL_NODE,
             {{ {W, 2, IN}, NC, {R, 4, IN}, {W, 9, OUT}, {W, 3, OUT}, {W, 8, IN} }},
             NULL_NODE,
             {{ {W, 5, OUT}, {B, 5, OUT}, {W, 4, IN}, {G, 6, OUT}, {B, 4, IN}, {G, 5, IN} }}, // absolute center
             NULL_NODE,
             {{ {B, 2, IN}, {B, 8, IN}, {B, 3, OUT}, {B, 9, OUT}, {G, 0, OUT}, NC }},
             NULL_NODE,
         }},

        {{
             // level 5
             { { NC, NC, NC, {R, 3, IN}, NC, {R, 4, OUT} }},
             NULL_NODE,
             {{ NC, {W, 3, IN}, NC, {R, 0, OUT}, NC, {W, 4, OUT} }},
             NULL_NODE,
             {{ NC, {B, 4, OUT}, NC, {G, 4, IN}, NC, {B, 3, IN} }},
             NULL_NODE,
             {{ NC, {G, 0, IN}, NC, {G, 1, OUT}, NC , NC}}
         }},

        {{
             // level 6
             NULL_NODE,
             {{ {W, 9, IN}, NC, {R, 5, OUT}, NC, {W, 10, OUT}, NC }},
             NULL_NODE,
             {{ {G, 6, IN}, NC, {G, 10, IN}, NC, {G, 7, OUT}, NC }},
             NULL_NODE,
             {{ {B, 9, IN}, NC, {G, 11, IN}, NC, {B, 10, OUT}, NC }},
             NULL_NODE
         }},

        {{
             // level 7
             {{ {R, 3, OUT}, NC, NC, NC, {R, 2, IN}, {R, 5, IN} }},
             NULL_NODE,
             {{ {R, 0, IN}, {W, 10, IN}, {R, 1, OUT}, NC, {G, 9, IN}, {G, 10, OUT} }},
             NULL_NODE,
             {{ {G, 4, OUT}, {G, 7, IN}, {G, 8, OUT}, NC, {G, 3, IN}, {G, 11, OUT} }},
             NULL_NODE,
             {{ {G, 1, IN}, {B, 10, IN}, {G, 2, OUT}, NC, NC, NC }}
         }},

        {{
             // level 8
             NULL_NODE,
             {{ NC, {R, 2, OUT}, NC, NC, NC, {R, 1, IN} }},
             NULL_NODE,
             { { NC, {G, 9, OUT}, NC, NC, NC, {G, 8, IN} }}, // absolute center top
             NULL_NODE,
             {{ NC, {G, 3, OUT}, NC, NC, NC, {G, 2, IN} }},
             NULL_NODE,
         }}
    };

}
