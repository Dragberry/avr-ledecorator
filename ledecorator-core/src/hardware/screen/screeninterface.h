#ifndef SCREENINTERFACE_H_
#define SCREENINTERFACE_H_

#include <avr/io.h>
#include <stdint.h>
#include "lib/screen/commands.h"
#include "lib/screen/definitions.h"
#include "../screendatainterface.h"

class ScreenInterface
{
private:
	const ScreenDataInterface& screen_data_interface;

	volatile uint8_t is_image_being_transmitted = 0;

	uint8_t buffer_1[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t buffer_2[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t(*active_buffer)[SCREEN_WIDTH] = buffer_1;
	uint8_t(*buffer)[SCREEN_WIDTH] = buffer_2;

	uint8_t y = 0;
	uint8_t x = 0;

public:
	ScreenInterface(ScreenDataInterface& screen_data_interface);

	inline void start_picture();

	inline void complete_picture();

	inline void swith_buffer();

	inline void send_byte();
};

#endif
