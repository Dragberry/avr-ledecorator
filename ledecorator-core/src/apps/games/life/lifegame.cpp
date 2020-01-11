#include "lifegame.hpp"

#include <stdlib.h>
#include "lib/screen/definitions.h"

LifeGame::LifeGame()
{
	clear_field();
	color_life = dragberry::io::read();
	color_dead = dragberry::io::read();;
	alive_indicator = ALIVE_INDICATOR_01;
	place_entity(0, 0, &SHIP_LIGHT, alive_indicator);
	place_entity(10, 0, &SHIP_MEDIUM, alive_indicator);
	place_entity(20, 0, &SHIP_LARGE, alive_indicator);
	is_step_required = 0;
	time = 0;
	Timers::T1::start(0x7A1, Timers::Prescaller::F_1024, this);
}

LifeGame::~LifeGame()
{
	Timers::T1::stop();
}

void LifeGame::clear_field()
{
	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			field[y][x] = 0;
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


	if (time >= 16 && time % 16 == 0)
	{
	    if (time % 32 != 0)
	    {
	        place_entity(15, 11, &GLIDER, next_alive_indicator);
	    }
	    else
	    {
	        place_entity(0, 8, &SHIP_MEDIUM, next_alive_indicator);
	    }
	}

	alive_indicator = next_alive_indicator;

}

void LifeGame::runner()
{
	LifeGame app;
	app.run();
}

void LifeGame::on_timer1_event()
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
			is_step_required = false;
		}
	}
	while (time <= 300);
}


void LifeGame::place_entity(
		const uint8_t start_x,
		const uint8_t start_y,
		const BitMap* data,
		const uint8_t next_alive
		)
{
	uint8_t width = data->get_width();
	uint8_t height = data->get_height();
	for (uint8_t y = 0; y < height; y++)
	{
		for (uint8_t x = 0; x < width; x++)
		{
			uint8_t state = data->get_bit(x, y) ? next_alive : 0;
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
			field[real_y][real_x] = state;
		}
	}
}
