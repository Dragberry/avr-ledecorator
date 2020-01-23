#include <stdlib.h>
#include "snakegame.h"

SnakeGame::SnakeGame() :
    time(0),
    field{0},
    head{0},
    tail{0}
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

    tail.x = 7;
    tail.y = 8;

    set(tail.x, tail.y,
            Type::SNAKE | SnakePart::TAIL | SnakeDirection::RIGHT);
    set(1, 8,
            Type::SNAKE | SnakePart::BODY | SnakeDirection::RIGHT);
    set(2, 8,
            Type::SNAKE | SnakePart::BODY | SnakeDirection::RIGHT);
    set(3, 8,
            Type::SNAKE | SnakePart::BODY | SnakeDirection::RIGHT);
    set(4, 8,
            Type::SNAKE | SnakePart::BODY | SnakeDirection::RIGHT);
    set(5, 8,
            Type::SNAKE | SnakePart::BODY | SnakeDirection::RIGHT);
    set(6, 8,
            Type::SNAKE | SnakePart::BODY | SnakeDirection::RIGHT);
    set(head.x, head.y,
            Type::SNAKE | SnakePart::HEAD | SnakeDirection::RIGHT);

    set(23, 11, Type::FOOD);
    set(13, 3, Type::FOOD);
    set(31, 15, Type::FOOD);
    set(2, 6, Type::FOOD);
    set(12, 9, Type::FOOD);

    do
    {
        move();
        draw();
        dragberry::os::display::update_requsted();
    }
    while (time <= 200);
    Timers::T1::stop();
}

void SnakeGame::on_timer1_event()
{
    time++;
    is_action_allowed = true;


}
