#include <stdlib.h>
#include <math.h>
#include "snakegame.h"

SnakeGame::SnakeGame() :
        time(0),
        steps(0),
        current_speed(MIN_SPEED),
        remaining_time(0),
        field{ 0 },
        head{ 0 },
        tail{ 0 },
        food{ 24, 11 }
{
}

void SnakeGame::runner()
{
    SnakeGame app;
    app.run();
}

void SnakeGame::run()
{
    place_snake(0, 8, 12);

    set(23, 11, Type::FOOD);
    set(13, 3, Type::FOOD);
    set(31, 15, Type::FOOD);
    set(2, 6, Type::FOOD);
    set(12, 9, Type::FOOD);

    set(food.x, food.y, Type::FOOD);

    current_speed = MAX_SPEED;
    refresh_remaining_time();
    Timers::T1::start(0x3D1, Timers::Prescaller::F_1024, this);
    do
    {
        make_step([&]() -> void
        {
            move();
            draw();
            dragberry::os::display::update_requsted();
        });
    } while (time <= 600);
    Timers::T1::stop();
}

void SnakeGame::on_timer1_event()
{
    time++;
    if (remaining_time > 0)
    {
        remaining_time--;
    }
}

void SnakeGame::move()
{
    bool is_tail_moved = true;

    SnakeDirection direction_next = make_decision();

    Point next = get_next(head, direction_next);

    uint8_t data = field[next.y][next.x];
    switch (data & MASK_TYPE)
    {
    case FIELD:
        break;
    case FOOD:
        eat();
        is_tail_moved = false;
        break;
    case WALL:
        break;
    case SNAKE:
        eat_yourself(next);
        is_tail_moved = false;
        break;
    }

    move_head(next, direction_next);
    if (is_tail_moved)
    {
      move_tail();
    }
}


SnakeGame::SnakeDirection SnakeGame::make_decision()
{
    PossibleStep possible_steps[3];
    SnakeDirection direction = get_direction(head);
    possible_step(direction, possible_steps[0]);
    possible_step(turn_left(direction), possible_steps[1]);
    possible_step(turn_right(direction), possible_steps[2]);

    sort(possible_steps, 3, [](PossibleStep& i, PossibleStep& j) -> bool {
        return i.possibility > j.possibility || (i.possibility == j.possibility ? i.distance > j.distance : false);
    });

//    if (steps % ((rand() % 4) + 3) == 0)
//        if (steps % 2 == 0)
//    {
//            return turn_left(direction);
//        switch (rand() % 3)
//        {
//        case 1:
//            return turn_left(direction);
//        case 2:
//            return turn_right(direction);
//        default:
//            return direction;
//        }
//    }
    return possible_steps[0].direction;
}

void SnakeGame::move_head(Point& next, SnakeDirection direction)
{
    set(head, Type::SNAKE | SnakePart::BODY | direction);
    set(next, Type::SNAKE | SnakePart::HEAD | direction);
    head = next;
}

void SnakeGame::move_tail()
{
    SnakeDirection direction_tail = get_direction(tail);
    Point next_tail = get_next(tail, direction_tail);
    SnakeDirection direction_tail_next = get_direction(next_tail);
    set(next_tail,  Type::SNAKE | SnakePart::TAIL | direction_tail_next);
    set(tail,       Type::FIELD | FieldType::GRASS);
    tail = next_tail;
}

void SnakeGame::eat()
{
    uint8_t attempts = 0;
    while (attempts++ < 10)
    {
       food.x = rand() % SCREEN_WIDTH;
       food.y = rand() % SCREEN_HEIGHT;
       uint8_t place_for_food = field[food.y][food.x];
       if ((place_for_food & MASK_TYPE) != Type::SNAKE)
       {
           set(food.x, food.y, Type::FOOD);
           break;
       }
    }
}

void SnakeGame::eat_yourself(const Point& next)
{
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
}

SnakeGame::SnakeDirection SnakeGame::turn_left(SnakeDirection direction)
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

SnakeGame::SnakeDirection SnakeGame::turn_right(SnakeDirection direction)
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


SnakeGame::Point SnakeGame::get_next(const Point& current, SnakeDirection direction)
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

SnakeGame::SnakeDirection SnakeGame::get_direction(const Point& point)
{
    return (SnakeDirection)(field[point.y][point.x] & MASK_SNAKE_DIRECTION);
}

void SnakeGame::set(const uint8_t x, const uint8_t y, const uint8_t data)
{
   field[y][x] = data;
}

void SnakeGame::set(const Point& point, const uint8_t data)
{
   field[point.y][point.x] = data;
}

void SnakeGame::place_snake(uint8_t start_x, uint8_t start_y, uint8_t length)
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

void SnakeGame::draw()
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
