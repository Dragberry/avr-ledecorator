#include <stdlib.h>
#include <string.h>
#include "lib/screen/definitions.h"
#include "entities.hpp"
#include "lifegame.hpp"

#define LIFE_GAME_TIME 300

LifeGame::LifeGame()
{
    is_random = rand() % 2;
	color_life = dragberry::io::read();
	color_dead = dragberry::io::read();;
	alive_indicator = ALIVE_INDICATOR_01;
	if (is_random)
	{
	    random_field();
	}
	else
	{
	    place_entity<5, 4>(0, 0, alive_indicator, &Entities::SHIP_LIGHT);
        place_entity<6, 5>(10, 0, alive_indicator, &Entities::SHIP_MEDIUM);
        place_entity<7, 5>(20, 0, alive_indicator, &Entities::SHIP_LARGE);
	}

	is_step_required = 0;
	time = 0;
	System::register_timer(this, 10);
}

LifeGame::~LifeGame()
{
    System::deregister_timer(this);
}

void LifeGame::random_field()
{
	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			field[y][x] = rand() % 2 ? alive_indicator : 0;
		}
	}
}

void LifeGame::build_scene()
{
	for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
	{
		for (uint8_t cell = 0; cell < SCREEN_WIDTH; cell++)
		{
			Color color = field[row][cell] & alive_indicator ? color_life : color_dead;
			dragberry::os::display::set_pixel(row, cell, color);
		}
	}
	dragberry::os::display::update_assured();
}

void LifeGame::step_up()
{
	uint8_t next_alive_indicator = alive_indicator ==
			ALIVE_INDICATOR_01 ? ALIVE_INDICATOR_10 : ALIVE_INDICATOR_01;
	uint8_t next_dead_indicator = ~next_alive_indicator;

	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
	{
		uint8_t* top_row = field[y == 0 ? SCREEN_HEIGHT - 1 : y - 1];
		uint8_t* current_row = field[y];
		uint8_t* bottom_row = field[y == SCREEN_HEIGHT - 1 ? 0 : y + 1];
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			uint8_t alive_neigbhoors = 0;
			uint8_t left = x == 0 ? SCREEN_WIDTH - 1 : x - 1;
			uint8_t right = x == SCREEN_WIDTH - 1 ? 0 : x + 1;

			if (top_row[left] & alive_indicator)
			{
				alive_neigbhoors++;
			}
			if (top_row[x] & alive_indicator)
			{
				alive_neigbhoors++;
			}
			if (top_row[right] & alive_indicator)
			{
				alive_neigbhoors++;
			}
			if (current_row[right] & alive_indicator)
			{
				alive_neigbhoors++;
			}
			if (bottom_row[right] & alive_indicator)
			{
				alive_neigbhoors++;
			}
			if (bottom_row[x] & alive_indicator)
			{
				alive_neigbhoors++;
			}
			if (bottom_row[left] & alive_indicator)
			{
				alive_neigbhoors++;
			}
			if (current_row[left] & alive_indicator)
			{
				alive_neigbhoors++;
			}

			uint8_t cell = field[y][x];
 			field[y][x] = (alive_neigbhoors == 3 || (alive_neigbhoors == 2 && (cell & alive_indicator))) ?
					cell | next_alive_indicator : cell & next_dead_indicator;
		}
	}

	if (!is_random)
	{
        if (time >= 16 && (time % (14 + rand() % 8)) == 0)
        {
            if (rand() % 2)
            {
                place_entity<3, 3>(15, 11, next_alive_indicator, &Entities::GLIDER);
            }
            else
            {
                place_entity<6, 5>(0, 8, next_alive_indicator, &Entities::SHIP_MEDIUM);
            }
        }
	}

	alive_indicator = next_alive_indicator;

}

void LifeGame::runner()
{
	LifeGame app;
	app.run();
}

void LifeGame::on_timer_event()
{
	time++;
	is_step_required = true;
}

void LifeGame::run()
{
	is_step_required = true;
	dragberry::os::display::clear_screen(BLACK);
	dragberry::os::display::update_assured();
	do
	{
		if (is_step_required)
		{
			build_scene();
			step_up();
			System::out::send_assured([&](RingBuffer<uint8_t, 20>& frame) -> void
            {
                char str[20] = { 0 };
                strcat(str, "Time: ");
                char value[8];
                strcat(str, ltoa(time, value, 10));

                uint8_t i = 0;
                while (str[i] != '\0')
                {
                    frame.add(str[i++]);
                }

                frame.add('\n');
            });
			is_step_required = false;
		}
	}
	while (time <= LIFE_GAME_TIME);
}
