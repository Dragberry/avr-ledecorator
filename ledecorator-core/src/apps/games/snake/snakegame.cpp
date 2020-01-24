#include <stdlib.h>
#include "snakegame.h"

SnakeGame::SnakeGame() :
        time(0),
        steps(0),
        current_speed(MIN_SPEED),
        remaining_time(0),
        field{ 0 },
        head{ 0 },
        tail{ 0 }
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
