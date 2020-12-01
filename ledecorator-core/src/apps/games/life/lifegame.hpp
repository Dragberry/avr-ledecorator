#ifndef LIFEGAME_HPP_
#define LIFEGAME_HPP_

#include <stdint.h>
#include <stdlib.h>
#include "../../../dragberry/os.hpp"
#include "../../../dragberry/os/display.hpp"
#include "../../../dragberry/os/io.hpp"

#define ALIVE_INDICATOR_01 0b00000001
#define ALIVE_INDICATOR_10 0b00000010

class LifeGame :
        public Application,
        public Timer
{
private:
    struct StoredState
    {
       enum Mode
       {
           RANDOM, SCRIPT, CAROUSEL
       };
       uint16_t time_to_live = 300;
       Color color_life = RED;
       Color color_dead = WHITE;
       Mode mode = CAROUSEL;
    };

    static const StoredState EEMEM STORED_STATE;

	uint8_t color_life;
	uint8_t color_dead;

	uint8_t alive_indicator;

	uint8_t field[SCREEN_HEIGHT][SCREEN_WIDTH] = { };

	uint16_t steps;

	volatile bool is_step_required;

	bool is_random;

public:
	LifeGame();

	~LifeGame();

private:
	void random_field();

	void build_scene();

	void step_up();

public:
	void run();

	void on_timer_event();

	template <uint8_t width, uint8_t height>
	void place_entity(
	        const uint8_t start_x,
	        const uint8_t start_y,
	        const uint8_t next_alive,
	        const BitMap<width * height>* data
	        )
	{
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

};

#endif
