#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "snakegame.h"
#include "../../../util/sorting.hpp"

const SnakeGame::StoredState EEMEM SnakeGame::STORED_STATE = SnakeGame::StoredState();

SnakeGame::SnakeGame() :
        steps(0),
        current_speed(MIN_SPEED),
        field{ 0 },
        head{ 0 },
        tail{ 0 },
        food(ArrayList<Point, 5>())
{
            eeprom_read_block((void*) &game_state, (const void*) &STORED_STATE, sizeof(STORED_STATE));
            time_to_live = game_state.time_to_live;
            current_speed = game_state.speed;
            is_step_required = true;
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

    System::register_timer(this, System::TIMER_FREQ / TICKS_PER_SECOND);
    do
    {
        if (!do_step())
        {
            break;
        }
    } while (is_going_on());
    System::deregister_timer();
}

bool SnakeGame::do_step()
{
    if (is_step_required)
    {
        if (!move())
        {
            return false;
        }

        draw();
        dragberry::os::display::update_assured();
        steps++;
        is_step_required = false;
    }
    return true;
}

void SnakeGame::on_timer_event()
{
    increment_time();
    if (time % current_speed == 0) {
        is_step_required = true;
    }
}

bool SnakeGame::move()
{
    volatile Mode mode = Mode::AUTO;
    volatile ManualCommand command = ManualCommand::IDLE;
    System::io::exchange(
        [&](char* frame) -> void
        {
            frame[1] = System::APP_SNAKE;
            System::io::decompose(time, 2);
            if (load_requested)
            {
                eeprom_read_block((void*) &game_state, (const void*) &STORED_STATE, sizeof(STORED_STATE));
                frame[4] = System::COMMAND_LOAD;
                System::io::decompose(game_state.time_to_live / TICKS_PER_SECOND, 5);
                frame[7] = game_state.field_color;
                frame[8] = game_state.snake_head_color;
                frame[9] = game_state.snake_body_color;
                frame[10] = game_state.snake_dead_color;
                frame[11] = game_state.food_increment_color;
                frame[12] = game_state.food_decrement_color;
                frame[13] = game_state.food_speed_up_color;
                frame[14] = game_state.food_speed_down_color;
                frame[15] = game_state.wall_color;
                frame[16] = game_state.wall;
                frame[17] = game_state.speed;
                load_requested = false;
            }
            else
            {
                frame[4] = System::COMMAND_EMPTY;
                frame[5] = mode;
                frame[6] = head.x;
                frame[7] = head.y;
                frame[8] = tail.x;
                frame[9] = tail.y;
                frame[10] = current_speed;
            }
        },
        [&](char* frame) -> void
        {
            mode = (Mode)frame[4];
            command = (ManualCommand)frame[5];
            switch (frame[3])
            {
            case System::COMMAND_LOAD:
                load_requested = true;
                break;
            case System::COMMAND_SAVE:
                System::io::compose(game_state.time_to_live, 4);
                game_state.time_to_live *= TICKS_PER_SECOND;
                game_state.field_color = frame[6];
                game_state.snake_head_color = frame[7];
                game_state.snake_body_color = frame[8];
                game_state.snake_dead_color = frame[9];
                game_state.food_increment_color = frame[10];
                game_state.food_decrement_color = frame[11];
                game_state.food_speed_up_color = frame[12];
                game_state.food_speed_down_color = frame[13];
                game_state.wall_color = frame[14];
                game_state.wall = (Wall) frame[15];
                game_state.speed = frame[16];
                eeprom_update_block((const void*) &game_state, (void*) &STORED_STATE, sizeof(STORED_STATE));
                load_requested = true;
                current_speed = game_state.speed;
                break;
            default:
                break;
            }
        }
    );

    SnakeDirection direction = get_direction(head);
    if (mode == Mode::MANUAL)
    {
        switch (command)
        {
        case ManualCommand::MOVE_LEFT:
            direction = turn_left(direction);
            break;
        case ManualCommand::MOVE_RIGHT:
            direction = turn_right(direction);
            break;
        default:
            break;
        }
    }
    else
    {
        make_decision(direction);
        PossibleStep next_step = possible_steps[0];
        if (next_step.priority == SnakeGame::PossibleStep::Priority::IMPOSSIBLE)
        {
            return false;
        }

        direction = next_step.direction;
    }

    Point next = get_next(head, direction);

    uint8_t data = field[next.y][next.x];
    switch (data & MASK_TYPE)
    {
    case FIELD:
        move_tail();
        move_head(next, direction);
        break;
    case FOOD:
        move_head(next, direction);
        eat(next, (FoodType)(data & MASK_FOOD_TYPE));
        break;
    case SNAKE:
        if (next == tail)
        {
            move_tail();
            move_head(next, direction);
        }
        else
        {
            eat_yourself(next);
            move_head(next, direction);
        }
        break;
    case WALL:
        return false;
    }
    return true;
}

void SnakeGame::make_decision(SnakeDirection direction)
{
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
    if (current_speed > MAX_SPEED)
    {
        current_speed--;
    }
}

void SnakeGame::slow_down()
{
    if (current_speed <= MIN_SPEED)
    {
        current_speed++;
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
    Wall wall = game_state.wall;
    if (wall == Wall::RANDOM)
    {
        wall = (Wall) (rand() % 4) ;
    }
    switch (wall)
    {
    case Wall::TUNNEL:
        place_wall(8, 4, &Walls::TUNNEL);
        place_wall(16, 4, &Walls::TUNNEL);
        break;
    case Wall::CROSS:
        place_wall(8, 4, &Walls::CROSS);
        place_wall(16, 4, &Walls::CROSS);
        break;
    case Wall::BRIDGE:
        place_wall(12, 0, &Walls::BRIDGE_TOP);
        place_wall(12, 8, &Walls::BRIDGE_BOTTOM);
        break;
    default:
        break;
    }
}

void SnakeGame::place_wall(
            const uint8_t start_x,
            const uint8_t start_y,
            const BitMap8x8* data)
{
    for (uint8_t y = 0; y < 8; y++)
    {
        for (uint8_t x = 0; x < 8; x++)
        {
            uint8_t real_y = start_y + y;
            if (real_y > SCREEN_HEIGHT)
            {
                real_y -=SCREEN_HEIGHT;
            }
            uint8_t real_x = start_x + x;
            if (real_x > SCREEN_WIDTH)
            {
                real_x -=SCREEN_WIDTH;
            }
            set(real_x, real_y, data->get_bit(x, y) ? Type::WALL : Type::FIELD);
        }
    }
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
                   color = game_state.snake_dead_color;
                   break;
               default:
                   color = game_state.field_color;
                   break;
               }
               break;
           case Type::SNAKE:
               switch (data & MASK_SNAKE_PART)
               {
               case SnakePart::BODY:
               case SnakePart::TAIL:
                   color = game_state.snake_body_color;
                   break;
               case SnakePart::HEAD:
                   color = game_state.snake_head_color;
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
                   color = game_state.food_increment_color;
                   break;
               case DECREMENT:
                   color = game_state.food_decrement_color;
                   break;
               case SPEED_UP:
                  color = game_state.food_speed_up_color;
                  break;
               case SLOW_DOWN:
                  color = game_state.food_speed_down_color;
               }
               break;
           case Type::WALL:
               color = game_state.wall_color;
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
