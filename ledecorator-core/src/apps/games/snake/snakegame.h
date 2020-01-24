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
    static const uint8_t TRASH_COLOR        = 0b00010101;
    static const uint8_t WALL_COLOR         = BLACK;
    static const uint8_t FOOD_COLOR         = RED;

    static const uint8_t MASK_TYPE = 0b00000011;

    enum Type : uint8_t
    {
        FIELD   = 0b00000000,
        SNAKE   = 0b00000001,
        FOOD    = 0b00000010,
        WALL    = 0b00000011,
    };

    static const uint8_t MASK_SNAKE_PART = 0b00001100;

    enum SnakePart : uint8_t
    {
        HEAD = 0b00000000,
        BODY = 0b00000100,
        TAIL = 0b00001000,
    };

    static const uint8_t MASK_SNAKE_DIRECTION = 0b00110000;

    enum SnakeDirection : uint8_t
    {
        UP      = 0b00000000,
        RIGHT   = 0b00010000,
        DOWN    = 0b00100000,
        LEFT    = 0b00110000,
    };

    static const uint8_t MASK_FIELD_TYPE = 0b00001100;

    enum FieldType : uint8_t
    {
        GRASS  = 0b00000000,
        LAKE   = 0b00000100,
        TRASH  = 0b00001000,
    };

    static const uint8_t MASK_FIELD_LIFETIME = 0b11110000;

    const static uint8_t MAX_SPEED = 20;
    const static uint8_t MIN_SPEED = 1;

    volatile uint16_t time;

    uint8_t steps = 0;

    uint8_t current_speed = MIN_SPEED;

    volatile uint8_t remaining_time = 0;

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
    inline
    void refresh_remaining_time()
    {
        remaining_time = MAX_SPEED + 1 - current_speed;
    }

    template <typename Step>
    void make_step(Step&& step)
    {
        if (remaining_time == 0)
        {
            step();
            refresh_remaining_time();
            steps++;
        }
    }

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
        bool move_tail = true;

        SnakeDirection direction_next = make_decision();

        Point next = get_next(head, direction_next);

        uint8_t data = field[next.y][next.x];
        switch (data & MASK_TYPE)
        {
        case FIELD:
            break;
        case FOOD:
            eat();
            move_tail = false;
            break;
        case WALL:
            break;
        case SNAKE:
            SnakeDirection new_tail_direction = get_direction(next);
            Point new_tail = get_next(next, new_tail_direction);


            Point current_dead_tail = tail;
            while (current_dead_tail.x != next.x || current_dead_tail.y != next.y)
            {
                SnakeDirection current_dead_tail_direction = get_direction(current_dead_tail);
                Point next_dead_tail = get_next(current_dead_tail, current_dead_tail_direction);
                set(current_dead_tail, Type::FIELD | FieldType::TRASH);
                current_dead_tail = next_dead_tail;
            }

            tail = new_tail;
            set(tail, Type::SNAKE | SnakePart::TAIL | get_direction(tail));
            move_tail = false;
            break;
        }

        set(head, Type::SNAKE | SnakePart::BODY | direction_next);
        set(next, Type::SNAKE | SnakePart::HEAD | direction_next);
        head = next;

        if (move_tail)
        {
            SnakeDirection direction_tail = get_direction(tail);
            Point next_tail = get_next(tail, direction_tail);
            SnakeDirection direction_tail_next = get_direction(next_tail);
            set(next_tail,  Type::SNAKE | SnakePart::TAIL | direction_tail_next);
            set(tail,       Type::FIELD | FieldType::GRASS);
            tail = next_tail;
        }
    }

    void eat()
    {
        uint8_t attempts = 0;
        while (attempts++ < 10)
        {
           uint8_t x = rand() % SCREEN_WIDTH;
           uint8_t y = rand() % SCREEN_HEIGHT;
           uint8_t place_for_food = field[y][x];
           if ((place_for_food & MASK_TYPE) != Type::SNAKE)
           {
               set(x, y, Type::FOOD);
               break;
           }
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
        if (steps % ((rand() % 4) + 3) == 0)
//        if (steps % 2 == 0)
        {
//            return turn_left(direction);
            switch (rand() % 3)
            {
            case 1:
                return turn_left(direction);
            case 2:
                return turn_right(direction);
            default:
                return direction;
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
                   switch(data & MASK_FIELD_TYPE)
                   {
                   case TRASH:
                       color = TRASH_COLOR;
                       break;
                   default:
                       color = FIELD_COLOR;
                       break;
                   }
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

    void place_snake(uint8_t start_x, uint8_t start_y, uint8_t length)
    {
        tail.x = start_x;
        tail.y = start_y;

        head.x = start_x + length;
        head.y = start_y;

        set(tail.x, tail.y, Type::SNAKE | SnakePart::TAIL | SnakeDirection::RIGHT);

        while (start_x < head.x)
        {
            set(++start_x, start_y, Type::SNAKE | SnakePart::BODY | SnakeDirection::RIGHT);
        }

        set(head.x, head.y, Type::SNAKE | SnakePart::HEAD | SnakeDirection::RIGHT);
    }
};

#endif
