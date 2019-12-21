#ifndef LIFEGAME_H_
#define LIFEGAME_H_

#include <stdint.h>
#include <stdlib.h>
#include "../../../dragberry/os.hpp"
#include "../../../dragberry/os/display.hpp"
#include "../../../dragberry/os/io.hpp"

#define ALIVE_INDICATOR_01 0b00000001
#define ALIVE_INDICATOR_10 0b00000010

class LifeGame :
		public dragberry::os::Program,
		public Timers::T1::Handler
{
private:
	uint8_t color_life;
	uint8_t color_dead;

	uint8_t alive_indicator;

	uint8_t field[SCREEN_HEIGHT][SCREEN_WIDTH];

public:
	LifeGame();

	~LifeGame();

	void run();

	void on_timer1_event();

	void place_ship(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive);

	void place_entity(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive);
};

#endif
