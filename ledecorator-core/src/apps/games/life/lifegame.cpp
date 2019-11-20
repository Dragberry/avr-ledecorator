#include "lifegame.h"
#include <stdlib.h>
#include "lib/screen/definitions.h"

LifeGame::LifeGame(uint8_t color_life, uint8_t color_dead)
{
	this->color_life = color_life;
	this->color_dead = color_dead;
	this->alive_indicator = ALIVE_INDICATOR_01;
	place_ship(0, 4, alive_indicator);
}

bool LifeGame::is_running()
{
	return time < 80;
}

void LifeGame::increment()
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
//	if (time % 8)
//	{
//		place_entity(8, 8, alive_indicator);
//	}

}

void LifeGame::build_image(ScreenInterface& screen_interface) const
{
	for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
	{
		for (uint8_t cell = 0; cell < SCREEN_WIDTH; cell++)
		{
			screen_interface.buffer[row][cell] =
					field[row][cell] & alive_indicator ? color_life : color_dead;
		}

	}
}
