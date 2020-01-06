#ifndef LIFEGAME_HPP_
#define LIFEGAME_HPP_

#include <stdint.h>
#include <stdlib.h>
#include "../../../dragberry/os/display.hpp"
#include "../../../dragberry/os/io.hpp"
#include "livingthings.hpp"

#define ALIVE_INDICATOR_01 0b00000001
#define ALIVE_INDICATOR_10 0b00000010

class LifeGame : public Timers::T1::Handler
{
private:
	uint8_t color_life;
	uint8_t color_dead;

	uint8_t alive_indicator;

	uint8_t field[SCREEN_HEIGHT][SCREEN_WIDTH];

	volatile uint16_t time;
	volatile bool is_step_required;

public:
	LifeGame();

	~LifeGame();

private:
	void clear_field();

	void build_scene();

	void step_up();

public:
	static void runner();

	void run();

	void on_timer1_event();

	void place_entity(
			const uint8_t start_x,
			const uint8_t start_y,
			const BitMap* data,
			const uint8_t next_alive
			);

	void place_ship(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive);

	void place_entity(const uint8_t start_x, const uint8_t start_y, const uint8_t next_alive);
};

#endif
