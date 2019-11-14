#ifndef SCREENINTERFACE_H_
#define SCREENINTERFACE_H_
#include <avr/io.h>
#include <stdint.h>
#include "lib/screen/colors.h"
#include "lib/screen/commands.h"
#include "lib/screen/definitions.h"
#include "../screendatainterface.h"
#include "../../data/characters.h"
#include "../../data/images.h"

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

	void clear_screen(const Color color);

	void draw_area(
			uint8_t start_x,
			uint8_t start_y,
			const uint8_t width,
			const uint8_t height,
			Color color);

	void draw_image(
			uint8_t start_x,
			uint8_t start_y,
			const Image& img,
			const Color bg_color);

	void draw_image(
			uint8_t start_x,
			uint8_t start_y,
			const ImageMono8x8& img,
			const Color color,
			const Color bg_color);

	void draw_number(
			uint8_t start_x,
			uint8_t start_y,
			float number,
			const bool sign,
			const uint8_t pr_int,
			const uint8_t pr_float,
			const Color color,
			const Color bg_color);

	void draw_histogram(
			uint8_t start_x,
			uint8_t start_y,
			const uint8_t width,
			const uint8_t height,
			const int16_t* data_set,
			const uint8_t data_set_size,
			const Color color,
			const Color bg_color);
};

#endif
