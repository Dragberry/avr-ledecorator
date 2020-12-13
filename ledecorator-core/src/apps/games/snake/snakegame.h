#ifndef SNAKEGAME_H_
#define SNAKEGAME_H_

#include "walls.hpp"
#include "../../../dragberry/os/application.hpp"
#include "../../../dragberry/os.hpp"
#include "../../../dragberry/os/display.hpp"
#include "../../../util/arraylist.hpp"

class SnakeGame :
        public Application,
        public Timer
{
public:
private:
    static const uint8_t TICKS_PER_SECOND = 25;

    enum Mode : char
    {
        AUTO      = 'A',
        MANUAL    = 'M',
    };

    enum ManualCommand : char
    {
        IDLE         = 'I',
        MOVE_LEFT    = 'L',
        MOVE_RIGHT   = 'R',
    };

    enum Wall : uint8_t
    {
        NO      = 0,
        CROSS   = 1,
        TUNNEL  = 2,
        BRIDGE  = 3,
        RANDOM  = 127,
    };

    struct StoredState
    {
        uint16_t time_to_live = 30 * TICKS_PER_SECOND;
        Color field_color = BLACK;
        Color snake_head_color = YELLOW;
        Color snake_body_color = WHITE;
        Color snake_dead_color = 0b00010101;
        Color food_increment_color = RED;
        Color food_decrement_color = BLUE;
        Color food_speed_up_color = MAGENTA;
        Color food_speed_down_color = GREEN;
        Color wall_color = 0b00000111;
        Wall wall = Wall::RANDOM;
        uint8_t speed = 3;
    };

    static const StoredState EEMEM STORED_STATE;

    StoredState game_state;

    uint8_t period = 0;

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

    static const uint8_t MASK_FOOD_TYPE = 0b00001100;

    enum FoodType : uint8_t
    {
        INCREMENT   = 0b00000000,
        DECREMENT   = 0b00000100,
        SPEED_UP    = 0b00001000,
        SLOW_DOWN   = 0b00001100,
    };

    const static uint8_t MAX_SPEED = 1;
    const static uint8_t MIN_SPEED = 40;

    uint8_t steps = 0;

    uint8_t current_speed = MIN_SPEED;

    volatile bool is_step_required;

    volatile bool load_requested = false;

    uint8_t field[SCREEN_HEIGHT][SCREEN_WIDTH];

    struct Point
    {
        int8_t x;
        int8_t y;

        inline
        bool operator == (Point& p)
        {
            return p.x == x && p.y == y;
        };

        inline
        bool operator != (Point& p)
        {
            return !(*this == p);
        };
    };

    Point head;
    Point tail;

    struct Distance
    {
        int8_t x;
        int8_t y;

        Distance(const Point& start, const Point& end);

        bool is_zero();

        int8_t full();
    };

    struct PossibleStep
    {
        enum Priority
        {
            PREFERRED,
            POSSIBLE,
            NOT_RECOMMENDED,
            IMPOSSIBLE
        } priority;

        SnakeDirection direction;

        int8_t distance;
    };

    PossibleStep possible_steps[3];

    ArrayList<Point, 5> food;

public:
	SnakeGame();

    void run();

    void on_timer_event();

private:
    bool do_step();

    void set(const uint8_t x, const uint8_t y, const uint8_t data);

    void set(const Point& point, const uint8_t data);

    bool move();

    void move_head(Point& next, SnakeDirection direction);

    void move_tail();

    void eat(Point& where_to_eat, FoodType what_to_eat);

    void speed_up();

    void slow_down();

    bool place_food();

    uint8_t get_food();

    void eat_yourself(const Point& next);

    uint8_t get_next(const Point& point);

    SnakeDirection get_direction(const Point& point);

    Point get_next(const Point& current, SnakeDirection direction);

    SnakeDirection turn_left(SnakeDirection direction);

    SnakeDirection turn_right(SnakeDirection direction);

    void make_decision(SnakeDirection direction);

    void possible_step(SnakeDirection direction, PossibleStep& step);

    void place_snake(uint8_t start_x, uint8_t start_y, uint8_t length);

    void place_walls();

    void draw();

    void place_wall(
            const uint8_t start_x,
            const uint8_t start_y,
            const BitMap8x8* data);
};

#endif
