#include <stdlib.h>
#include "snakegame.h"

SnakeGame::SnakeGame() :
    time(0),
    field{0}
{
}

void SnakeGame::runner()
{
    SnakeGame app;
    app.run();
}

void SnakeGame::run()
{
    Timers::T1::start(0x7A1, Timers::Prescaller::F_1024, this);

    head.x = 2;
    head.y = 8;

    tail.x = 0;
    tail.y = 8;

    field[tail.y][tail.x] =
            Type::SNAKE | (SnakePart::TAIL << 3) | (SnakeDirection::RIGHT << 5);
    field[8][1] =
            Type::SNAKE | (SnakePart::BODY << 3) | (SnakeDirection::RIGHT << 5);
    field[head.y][head.x] =
            Type::SNAKE | (SnakePart::HEAD << 3) | (SnakeDirection::RIGHT << 5);

    do
    {
        uint8_t head_value = field[head.y][head.x];
        SnakeDirection direction = (SnakeDirection)((head_value & MASK_SNAKE_DIRECTION) >> 5);
        switch ((head_value & MASK_SNAKE_DIRECTION) >> 5)
        {
        case SnakeDirection::UP:
            if (head.y == 0)
            {
                head.y = SCREEN_HEIGHT - 1;
            }
            else
            {
                head.y--;
            }
            break;
        case SnakeDirection::RIGHT:
            if (head.x == SCREEN_WIDTH - 1)
            {
                head.x = 0;
            }
            else
            {
                head.x++;
            }
            break;
        case SnakeDirection::DOWN:
            if (head.y == SCREEN_HEIGHT - 1)
            {
                head.y = 0;
            }
            else
            {
                head.y++;
            }
            break;
        case SnakeDirection::LEFT:
            if (head.x == 0)
            {
                head.x = SCREEN_WIDTH - 1;
            }
            else
            {
                head.x--;
            }
            break;
        default:
            break;
        }
        field[head.y][head.x] =
                Type::SNAKE | (SnakePart::HEAD << 3) | (direction << 5);

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
                    switch ((data & MASK_SNAKE_PART) >> 3)
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
        dragberry::os::display::update_assured();
    }
    while (time <= 100);
    Timers::T1::stop();
}

void SnakeGame::on_timer1_event()
{
    time++;
}
