#include "lifegame.h"
#include <stdlib.h>
#include "../../../screen/definitions.h"

LifeGame::LifeGame(uint8_t color_life, uint8_t color_dead)
{
	this->color_life = color_life;
	this->color_dead = color_dead;
	this->alive_indicator = ALIVE_INDICATOR_01;
	this->field = new uint8_t*[SCREEN_HEIGHT];
	for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
	{
		this->field[row] = new uint8_t[SCREEN_WIDTH];
	}
}

LifeGame::~LifeGame()
{
	for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
	{
		delete [] this->field[row];
	}
	delete [] this->field;
}

bool LifeGame::is_running()
{
	return true;
}


void LifeGame::increment()
{
	uint8_t next_alive_indicator = alive_indicator ==
			ALIVE_INDICATOR_01 ? ALIVE_INDICATOR_10 : ALIVE_INDICATOR_01;
	uint8_t next_dead_indicator = ~next_alive_indicator;

	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
	{
		uint8_t top = y == 0 ? SCREEN_HEIGHT - 1 : y - 1;
		uint8_t bottom = y == SCREEN_HEIGHT - 1 ? 0 : y + 1;
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			uint8_t alive_neigbhoors = 0;
			uint8_t left = x == 0 ? SCREEN_WIDTH - 1 : x - 1;
			uint8_t right = x == SCREEN_WIDTH - 1 ? 0 : x + 1;


			alive_neigbhoors += (field[top][left] & alive_indicator ? 1 : 0); 		// TOP LEFT
			alive_neigbhoors += (field[top][x] & alive_indicator ? 1 : 0); 			// TOP
			alive_neigbhoors += (field[top][right] & alive_indicator ? 1 : 0); 		// TOP RIGHT
			alive_neigbhoors += (field[y][right] & alive_indicator ? 1 : 0); 		// RIGHT
			alive_neigbhoors += (field[bottom][right] & alive_indicator ? 1 : 0); 	// BOTTOM RIGHT
			alive_neigbhoors += (field[bottom][x] & alive_indicator ? 1 : 0);		// BOTTOM
			alive_neigbhoors += (field[bottom][left] & alive_indicator ? 1 : 0); 	// BOTTOM LEFT
			alive_neigbhoors += (field[y][left] & alive_indicator ? 1 : 0); 		// LEFT

			uint8_t cell = field[y][x];

 			field[y][x] = (alive_neigbhoors == 3 || (alive_neigbhoors == 2 && (cell & alive_indicator))) ?
					cell | next_alive_indicator : cell & next_dead_indicator;
		}
	}
	alive_indicator = next_alive_indicator;
	if (time % 15 == 0)
	{
		place_entity(0, 0, alive_indicator);
	}

}

void LifeGame::build_image(uint8_t** image)
{
	for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
	{
		for (uint8_t cell = 0; cell < SCREEN_WIDTH; cell++)
		{
			image[row][cell] = this->field[row][cell] & alive_indicator ? color_life : color_dead;
		}

	}
}
