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
	uint8_t buffer_1[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t buffer_2[SCREEN_HEIGHT][SCREEN_WIDTH];

	uint8_t y = 0;
	uint8_t x = 0;

public:
	ScreenDataInterface& screen_data_interface;

	volatile uint8_t is_image_being_transmitted = 0;

	uint8_t(*active_buffer)[SCREEN_WIDTH] = buffer_1;
	uint8_t(*buffer)[SCREEN_WIDTH] = buffer_2;

	ScreenInterface(ScreenDataInterface& screen_data_interface);

	void start_picture();

	void send_next_byte();

	void switch_buffer();

	void complete_picture();
};

#endif