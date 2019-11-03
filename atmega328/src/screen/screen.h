#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>
#include "interfaces/coreinterface.h"
#include "interfaces/datainterface.h"
#include "interfaces/displayinterface.h"
#include "colors.h"
#include "definitions.h"
#include "row.h"
#include "section.h"

class Screen
{
private:
	uint8_t** buffer_1;
	uint8_t** buffer_2;

	volatile uint8_t is_being_read;

	Row current_row;

	uint8_t rows_state;

	void apply_colors(Colors& colors, const uint8_t color, const uint8_t current_bit);

	void apply_colors(Section& section, const uint8_t color, const uint8_t offset);

public:
	const CoreInterface* core_interface;
	const DataInterface* data_interface;
	const DisplayInterface* display_interface;

	uint8_t** buffer;
	uint8_t** active_buffer;

	Screen(
			const CoreInterface* core_interface,
			const DataInterface* data_interface,
			const DisplayInterface* display_interface
			);

	void launch();

	void stop();

	void switch_buffer();

	inline void start_reading();

	inline void stop_reading();

	void draw_row();
};

#endif
