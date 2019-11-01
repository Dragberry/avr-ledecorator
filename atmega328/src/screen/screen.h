#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>
#include "interfaces/datainterface.h"
#include "interfaces/displayinterface.h"
#include "interfaces/timerinterface.h"
#include "colors.h"
#include "definitions.h"
#include "row.h"
#include "section.h"

class Screen
{
private:
	DataInterface* data_interface;
	DisplayInterface* display_interface;
	TimerInterface* timer_interface;

	uint8_t** buffer_1;
	uint8_t** buffer_2;

	volatile uint8_t is_being_read;

	Row current_row;

	uint8_t rows_state;

	void apply_colors(Colors& colors, const uint8_t color, const uint8_t current_bit);

	void apply_colors(Section& section, const uint8_t color, const uint8_t offset);

public:
	uint8_t** buffer;
	uint8_t** active_buffer;

	Screen(DisplayInterface* display_interface, DataInterface* data_interface, TimerInterface* timer_interface);

	void launch();

	void stop();

	void switch_buffer();

	inline void start_reading();

	inline void stop_reading();

	void draw_row();
};

#endif
