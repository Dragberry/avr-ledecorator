#include <stdlib.h>
#include <math.h>
#include "snakegame.h"
#include "../../../util/sorting.hpp"

SnakeGame::SnakeGame() :
        time(0),
        steps(0),
        current_speed(MIN_SPEED),
        remaining_time(0),
        field{ 0 },
        head{ 0 },
        tail{ 0 },
        food(ArrayList<Point, 5>())
{
}

void SnakeGame::runner()
{
    SnakeGame app;
    app.run();
}

void SnakeGame::run()
{
    place_walls();
    place_snake(0, 8, 5);

    place_food();
    place_food();
    place_food();
    place_food();
    place_food();

    current_speed = MAX_SPEED - 2;
    refresh_remaining_time();
    // 0x1E9
    // 0x3D1
    // 0x7A
    Timers::T1::start(0x7A, Timers::Prescaller::F_1024, this);
    do
    {
        if (!do_step())
        {
            break;
        }
    } while (time <= 65535);
    Timers::T1::stop();
}

bool SnakeGame::do_step()
{
    if (remaining_time == 0)
    {
        if (!move())
        {
            return false;
        }
        draw();
        dragberry::os::display::update_requsted();
        refresh_remaining_time();
        steps++;
    }
    return true;
}

void SnakeGame::on_timer1_event()
{
    time++;
    if (remaining_time > 0)
    {
        remaining_time--;
    }
}

bool SnakeGame::move()
{
    make_decision();
    PossibleStep next_step = possible_steps[0];
    if (next_step.priority == SnakeGame::PossibleStep::Priority::IMPOSSIBLE)
    {
        return false;
    }

    Point next = get_next(head, next_step.direction);
    uint8_t data = field[next.y][next.x];
    switch (data & MASK_TYPE)
    {
    case FIELD:
        move_tail();
        move_head(next, next_step.direction);
        break;
    case FOOD:
        move_head(next, next_step.direction);
        eat(next, (FoodType)(data & MASK_FOOD_TYPE));
        break;
    case SNAKE:
        if (next == tail)
        {
            move_tail();
            move_head(next, next_step.direction);
        }
        else
        {
            eat_yourself(next);
            move_head(next, next_step.direction);
        }
        break;
    case WALL:
        return false;
    }
    return true;
}

void SnakeGame::make_decision()
{
    SnakeDirection direction = get_direction(head);
    possible_step(direction, possible_steps[0]);
    possible_step(turn_left(direction), possible_steps[1]);
    possible_step(turn_right(direction), possible_steps[2]);

    sort(possible_steps, 3, [](PossibleStep& i, PossibleStep& j) -> bool {
        if (i.priority > j.priority)
        {
            return true;
        }
        if (i.priority == j.priority)
        {
            if (i.distance > j.distance)
            {
                return true;
            }
            else if(i.distance == j.distance)
            {
                return rand() % 2;
            }
        }
        return false;
    });
}

void SnakeGame::possible_step(SnakeDirection direction, PossibleStep& step)
{
   step.direction = direction;
   Point point = get_next(head, direction);
   Distance distance = Distance(point, food.get(0));
   switch (field[point.y][point.x] & MASK_TYPE)
   {
   case FIELD:
       step.priority = PossibleStep::Priority::POSSIBLE;
       break;
   case FOOD:
       step.priority = PossibleStep::Priority::PREFERRED;
       break;
   case SNAKE:
       step.priority = PossibleStep::Priority::NOT_RECOMMENDED;
       break;
   case WALL:
       step.priority = PossibleStep::Priority::IMPOSSIBLE;
   }
   step.distance = distance.full();
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
    set(next_tail, Type::SNAKE | SnakePart::TAIL | direction_tail_next);
    set(tail, Type::FIELD | FieldType::GRASS);
    tail = next_tail;
}

void SnakeGame::eat(Point& where_to_eat, FoodType what_to_eat)
{
    food.remove(where_to_eat);
    place_food();
    switch (what_to_eat)
    {
    case INCREMENT:
        break;
    case DECREMENT:
        move_tail();
        move_tail();
        break;
    case SPEED_UP:
        speed_up();
        break;
    case SLOW_DOWN:
        slow_down();
        break;
    }
}

void SnakeGame::speed_up()
{
    if (current_speed < MAX_SPEED)
    {
        current_speed++;
    }
}

void SnakeGame::slow_down()
{
    if (current_speed > 0)
    {
        current_speed--;
    }
}

bool SnakeGame::place_food()
{
    const int8_t init_x = 20;
    const int8_t init_y = 8;

    int8_t x = rand() % SCREEN_WIDTH;
    int8_t y = rand() % SCREEN_HEIGHT;

    while (true)
    {
        uint8_t place_for_food = field[y][x];
        if (Type::FIELD == (place_for_food & MASK_TYPE))
        {
            Point place = { x, y };
            set(place, get_food());
            food.add(place);
            return true;
        }
        else
        {
            if (++x == SCREEN_WIDTH)
            {
               x = 0;
               if (++y == SCREEN_HEIGHT)
               {
                   y = 0;
               }
            }
            if (x == init_x && y == init_y)
            {
               return false;
            }
        }
    }
}

uint8_t SnakeGame::get_food()
{
    FoodType food_type;
    uint8_t temp = (rand() % 10);
    if (temp < 6)
    {
        food_type = FoodType::INCREMENT;
    }
    else if (temp < 8)
    {
        food_type = FoodType::DECREMENT;
    }
    else if (temp < 9)
    {
        food_type = FoodType::SPEED_UP;
    }
    else
    {
        food_type = FoodType::SLOW_DOWN;
    }
    return Type::FOOD | food_type;
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

void SnakeGame::place_walls()
{
//    place_wall<12, 8>(10, 4, &Walls::TUNNEL);
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
               case SnakePart::TAIL:
                   color = SNAKE_COLOR;
                   break;
               case SnakePart::HEAD:
                   color = SNAKE_HEAD_COLOR;
                   break;
                   break;
               default:
                   break;
               }
               break;
           case Type::FOOD:
               switch (data & MASK_FOOD_TYPE)
               {
               case INCREMENT:
                   color = FOOD_INCREMENT_COLOR;
                   break;
               case DECREMENT:
                   color = FOOD_DECREMENT_COLOR;
                   break;
               case SPEED_UP:
                  color = FOOD_SPEED_UP_COLOR;
                  break;
               case SLOW_DOWN:
                  color = FOOD_SPEED_DOWN_COLOR;
               }
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

SnakeGame::Distance::Distance(const Point& start, const Point& end)
{
    x = abs(start.x - end.x);
    if (x >= SCREEN_WIDTH / 2)
    {
        x = SCREEN_WIDTH - x;
    }
    y = abs(start.y - end.y);
    if (y >= SCREEN_HEIGHT / 2)
    {
        y = SCREEN_HEIGHT - y;
    }
}

inline
bool SnakeGame::Distance::is_zero()
{
    return x == 0 && y == 0;
}

inline
int8_t SnakeGame::Distance::full()
{
    return x + y;
}
