#ifndef LIFEGAME_H_
#define LIFEGAME_H_

#include <stdint.h>
#include <stdlib.h>
#include "../../application.h"

#define ALIVE_INDICATOR_01 0b00000001
#define ALIVE_INDICATOR_10 0b00000010

class LifeGame : public Application
{
private:
	uint8_t color_life;
	uint8_t color_dead;

	uint8_t alive_indicator;

	uint8_t field[SCREEN_HEIGHT][SCREEN_WIDTH] = {{0}};

protected:
	void increment();
	void onAction();

public:
	LifeGame(uint8_t color_life, uint8_t color_dead);
	~LifeGame() {}

	bool is_running();

	void build_image(uint8_t(*image)[SCREEN_WIDTH]);

	void place_ship(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive)
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

	void place_entity(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive)
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
};

#endif
