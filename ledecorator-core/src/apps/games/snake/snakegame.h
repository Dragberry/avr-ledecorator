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
        int8_t x;
        int8_t y;
    };

    Point head;
    Point tail;

    Point food;

    struct Distance
    {
        int8_t x;
        int8_t y;

        Distance(const Point& start, const Point& end)
        {
            x = abs(start.x - end.x);
            if (x > SCREEN_WIDTH / 2)
            {
                x = SCREEN_WIDTH - x;
            }
            y = abs(start.y - end.y);
            if (y > SCREEN_HEIGHT / 2)
            {
                y = SCREEN_HEIGHT - y;
            }
        }

        inline
        bool is_zero()
        {
            return x == 0 && y == 0;
        }

        inline
        int8_t full()
        {
            return x + y;
        }
    };

    struct PossibleStep
    {
        enum Quality
        {
            Good, Normal, Bad
        } possibility;

        SnakeDirection direction;

        int8_t distance;
    };

    void possible_step(SnakeDirection direction, PossibleStep& step)
    {
        step.direction = direction;
        Point point = get_next(head, direction);
        Distance distance = Distance(point, food);
        switch (field[point.y][point.x] & MASK_TYPE)
        {
        case FIELD:
        case FOOD:
            step.possibility = PossibleStep::Quality::Good;
            break;
        case SNAKE:
            step.possibility = PossibleStep::Quality::Normal;
            break;
        case WALL:
            step.possibility = PossibleStep::Quality::Bad;
        }
        step.distance = distance.full();
    }

    template <typename Type, typename Comparator>
    void sort(Type* array, const int size, Comparator&& compare)
    {
        for (uint8_t i = 0; i < (size - 1); i++)
        {
            for (uint8_t j = i + 1; j < size; j++)
            {
                if (compare(array[i], array[j]))
                {
                    Type temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                }
            }
        }
    }

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

    void set(const uint8_t x, const uint8_t y, const uint8_t data);

    void set(const Point& point, const uint8_t data);

    void move();

    void move_head(Point& next, SnakeDirection direction);

    void move_tail();

    void eat();

    void eat_yourself(const Point& next);

    uint8_t get_next(const Point& point);

    SnakeDirection get_direction(const Point& point);

    Point get_next(const Point& current, SnakeDirection direction);

    SnakeDirection turn_left(SnakeDirection direction);

    SnakeDirection turn_right(SnakeDirection direction);

    SnakeDirection make_decision();

    void draw();

    void place_snake(uint8_t start_x, uint8_t start_y, uint8_t length);
};

#endif
