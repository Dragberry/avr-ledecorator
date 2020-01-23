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
        FIELD   = 0b00000000,
        SNAKE   = 0b00000001,
        FOOD    = 0b00000010,
        WALL    = 0b00000011,
    };

    static const uint8_t MASK_SNAKE_PART = 0b00011000;

    enum SnakePart : uint8_t
    {
        HEAD = 0b00000000,
        BODY = 0b00001000,
        TAIL = 0b00010000,
    };

    static const uint8_t MASK_SNAKE_DIRECTION = 0b01100000;

    enum SnakeDirection : uint8_t
    {
        UP      = 0b00000000,
        RIGHT   = 0b00100000,
        DOWN    = 0b01000000,
        LEFT    = 0b01100000,
    };

    volatile bool is_action_allowed = false;
    volatile uint16_t time;
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

private:
    void set(const uint8_t x, const uint8_t y, const uint8_t data)
    {
        field[y][x] = data;
    }

    void set(const Point& point, const uint8_t data)
    {
        field[point.y][point.x] = data;
    }

    void move()
    {
        bool has_grown_up = false;

        SnakeDirection direction_next = make_decision();

        Point next = get_next(head, direction_next);

        uint8_t data = field[next.y][next.x];
        switch (data & MASK_TYPE)
        {
        case FIELD:
            break;
        case FOOD:
            has_grown_up = true;
            break;
        case WALL:
            break;
        case SNAKE:
            break;
        }

        set(head,
                Type::SNAKE | SnakePart::BODY | direction_next);
        set(next,
                Type::SNAKE | SnakePart::HEAD | direction_next);
        head.x = next.x;
        head.y = next.y;

        if (!has_grown_up)
        {
            SnakeDirection direction_tail = get_direction(tail);
            Point next_tail = get_next(tail, direction_tail);
            SnakeDirection direction_tail_next = get_direction(next_tail);
            set(next_tail,
                    Type::SNAKE | SnakePart::TAIL | direction_tail_next);
            set(tail,
                    Type::FIELD);
            tail.x = next_tail.x;
            tail.y = next_tail.y;
        }
    }

    SnakeDirection get_direction(const Point& point)
    {
        return (SnakeDirection)(field[point.y][point.x] & MASK_SNAKE_DIRECTION);
    }

    Point get_next(const Point& current, SnakeDirection direction)
    {
        Point next;
        switch (direction)
        {
        case SnakeDirection::UP:
            next.y = current.y == 0 ? SCREEN_HEIGHT - 1 : current.y - 1;
            next.x = current.x;
            break;
        case SnakeDirection::RIGHT:
            next.x = current.x == SCREEN_WIDTH - 1 ? 0 : current.x + 1;
            next.y = current.y;
            break;
        case SnakeDirection::DOWN:
            next.y = current.y == SCREEN_HEIGHT - 1 ? 0 : current.y + 1;
            next.x = current.x;
            break;
        case SnakeDirection::LEFT:
            next.x = current.x == 0 ? SCREEN_WIDTH - 1 : current.x - 1;
            next.y = current.y;
            break;
        default:
            break;
        }
        return next;
    }

    SnakeDirection turn_left(SnakeDirection direction)
    {
        switch (direction)
        {
        case UP:
            return LEFT;
        case LEFT:
           return DOWN;
        case DOWN:
           return RIGHT;
        case RIGHT:
           return UP;
        }
        return direction;
    }

    SnakeDirection turn_right(SnakeDirection direction)
    {
        switch (direction)
        {
        case UP:
            return RIGHT;
        case RIGHT:
           return DOWN;
        case DOWN:
           return LEFT;
        case LEFT:
           return UP;
        }
        return direction;
    }

    SnakeDirection make_decision()
    {
        SnakeDirection direction = get_direction(head);
        if (is_action_allowed)
        {
            if (time % 4 == 0)
            {
                is_action_allowed = false;
                switch (random() % 3)
                {
                case 1:
                    return turn_left(direction);
                case 2:
                    return turn_right(direction);
                default:
                    return direction;
                }
            }
        }
        return direction;
    }

    void draw()
    {
        for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
       {
           for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
           {
               uint8_t color = BLACK;
               uint8_t data = field[y][x];
               uint8_t type = data & MASK_TYPE;
               switch (type)
               {
               case Type::FIELD:
                   color = FIELD_COLOR;
                   break;
               case Type::SNAKE:
                   switch (data & MASK_SNAKE_PART)
                   {
                   case SnakePart::BODY:
                       color = SNAKE_COLOR;
                       break;
                   case SnakePart::HEAD:
                       color = SNAKE_HEAD_COLOR;
                       break;
                   case SnakePart::TAIL:
                       color = SNAKE_COLOR;
                       break;
                   default:
                       break;
                   }
                   break;
               case Type::FOOD:
                   color = FOOD_COLOR;
                   break;
               case Type::WALL:
                   color = WALL_COLOR;
                   break;
               default:
                   break;
               }

               dragberry::os::display::set_pixel(y, x, color);
           }
       }
    }
};

#endif
