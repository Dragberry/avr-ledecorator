#ifndef SNAKEGAME_H_
#define SNAKEGAME_H_

#include "lib/avr/hardware/timers.hpp"
#include "../../../dragberry/os/display.hpp"

class SnakeGame : public Timers::T1::Handler
{
private:
    static const uint8_t FIELD_COLOR        = GREEN;
    static const uint8_t SNAKE_COLOR        = WHITE;
    static const uint8_t SNAKE_HEAD_COLOR   = YELLOW;
    static const uint8_t WALL_COLOR         = BLACK;
    static const uint8_t FOOD_COLOR         = RED;

    static const uint8_t MASK_TYPE      = 0b00000111;
    static const uint8_t MASK_SUBTYPE   = 0b11111000;

    enum Type : uint8_t
    {
        FIELD   = 0b000,
        SNAKE   = 0b001,
        FOOD    = 0b010,
        WALL    = 0b011,
    };

    static const uint8_t MASK_SNAKE_PART = 0b00011000;

    enum SnakePart : uint8_t
    {
        HEAD = 0b00,
        BODY = 0b01,
        TAIL = 0b10
    };

    static const uint8_t MASK_SNAKE_DIRECTION = 0b01100000;

    enum SnakeDirection : uint8_t
    {
        UP      = 0b00,
        RIGHT   = 0b01,
        DOWN    = 0b10,
        LEFT    = 0b11,
    };

    uint16_t time;
    uint8_t field[SCREEN_HEIGHT][SCREEN_WIDTH];

    struct Point
    {
        uint8_t x;
        uint8_t y;
    };

    Point head;
    Point tail;

public:
	SnakeGame();

    static void runner();

    void run();

    void on_timer1_event();
};

#endif
