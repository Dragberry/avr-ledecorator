#include "lifegame.hpp"

#include <stdlib.h>
#include "lib/screen/definitions.h"

LifeGame::LifeGame()
{
	clear_field();
	color_life = dragberry::io::read();
	color_dead = dragberry::io::read();;
	alive_indicator = ALIVE_INDICATOR_01;
	place_ship(0, 4, alive_indicator);
	// 0.1 second
	Timers::T1::start(0x4C5, Timers::Prescaller::F_1024, this);
}

LifeGame::~LifeGame()
{
	Timers::T1::stop();
}

void LifeGame::runner()
{
	LifeGame app;
	app.run();
}

void LifeGame::on_timer1_event()
{
	time++;
}

void LifeGame::run()
{
	while (time < 255)
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
		alive_indicator = next_alive_indicator;

		for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
		{
			for (uint8_t cell = 0; cell < SCREEN_WIDTH; cell++)
			{
				Color color = field[row][cell] & alive_indicator ? color_life : color_dead;
				dragberry::os::display::set_pixel(row, cell, color);

			}
		}

		dragberry::os::display::update_pending();
	}
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


void LifeGame::place_ship(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive)
{
	uint8_t x = start_x;
	field[start_y][x] = 0;
	field[start_y + 1][x] = next_alive;
	field[start_y + 2][x] = next_alive;
	field[start_y + 3][x] = 0;
	field[start_y + 4][x] = 0;
	field[start_y + 5][x] = next_alive;
	field[start_y + 6][x] = next_alive;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = next_alive;
	field[start_y + 4][x] = next_alive;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = next_alive;
	field[start_y + 4][x] = next_alive;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = next_alive;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = next_alive;
	field[start_y + 3][x] = 0;
	field[start_y + 4][x] = 0;
	field[start_y + 5][x] = next_alive;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = next_alive;
	x++;
	field[start_y + 0][x] = next_alive;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = 0;
	field[start_y + 4][x] = 0;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = next_alive;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = 0;
	field[start_y + 4][x] = 0;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = next_alive;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = 0;
	field[start_y + 4][x] = 0;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = next_alive;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = next_alive;
	field[start_y + 2][x] = next_alive;
	field[start_y + 3][x] = 0;
	field[start_y + 4][x] = 0;
	field[start_y + 5][x] = next_alive;
	field[start_y + 6][x] = next_alive;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = next_alive;
	field[start_y + 3][x] = next_alive;
	field[start_y + 4][x] = next_alive;
	field[start_y + 5][x] = next_alive;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = 0;
	field[start_y + 4][x] = 0;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = next_alive;
	field[start_y + 4][x] = next_alive;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = 0;
	x++;
	field[start_y + 0][x] = 0;
	field[start_y + 1][x] = 0;
	field[start_y + 2][x] = 0;
	field[start_y + 3][x] = next_alive;
	field[start_y + 4][x] = next_alive;
	field[start_y + 5][x] = 0;
	field[start_y + 6][x] = 0;
	field[start_y + 7][x] = 0;
}

void LifeGame::place_entity(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive)
{
	uint8_t direction = rand() % 4;
	switch (direction)
	{
	case 0:
		// 010
		// 001
		// 111
		field[start_y][start_x] = 0;
		field[start_y][start_x + 1] = next_alive;
		field[start_y][start_x + 2] = 0;

		field[start_y + 1][start_x] = 0;
		field[start_y + 1][start_x + 1] = 0;
		field[start_y + 1][start_x + 2] = next_alive;

		field[start_y + 2][start_x] = next_alive;
		field[start_y + 2][start_x + 1] = next_alive;
		field[start_y + 2][start_x + 2] = next_alive;
		break;
	case 1:
		// 100
		// 101
		// 110
		field[start_y][start_x] = next_alive;
		field[start_y][start_x + 1] = 0;
		field[start_y][start_x + 2] = 0;

		field[start_y + 1][start_x] = next_alive;
		field[start_y + 1][start_x + 1] = 0;
		field[start_y + 1][start_x + 2] = next_alive;

		field[start_y + 2][start_x] = next_alive;
		field[start_y + 2][start_x + 1] = next_alive;
		field[start_y + 2][start_x + 2] = 0;
		break;
	case 2:
		// 111
		// 100
		// 010
		field[start_y][start_x] = next_alive;
		field[start_y][start_x + 1] = next_alive;
		field[start_y][start_x + 2] = next_alive;

		field[start_y + 1][start_x] = next_alive;
		field[start_y + 1][start_x + 1] = 0;
		field[start_y + 1][start_x + 2] = 0;

		field[start_y + 2][start_x] = 0;
		field[start_y + 2][start_x + 1] = next_alive;
		field[start_y + 2][start_x + 2] = 0;
		break;
	case 3:
		// 011
		// 101
		// 001
		field[start_y][start_x] = 0;
		field[start_y][start_x + 1] = next_alive;
		field[start_y][start_x + 2] = next_alive;

		field[start_y + 1][start_x] = next_alive;
		field[start_y + 1][start_x + 1] = 0;
		field[start_y + 1][start_x + 2] = next_alive;

		field[start_y + 2][start_x] = 0;
		field[start_y + 2][start_x + 1] = 0;
		field[start_y + 2][start_x + 2] = next_alive;
		break;
	}
}
