#include "../../data/characters.h"
#include "screeninterface.h"
#include "math.h"

ScreenInterface::ScreenInterface(ScreenDataInterface& screen_data_interface) : screen_data_interface(screen_data_interface) {}

void ScreenInterface::start_picture()
{
	screen_data_interface.send_byte(COMMAND_MASK | CMD_DEFAULT);
	is_image_being_transmitted = 1;
}

void ScreenInterface::send_next_byte()
{
	if (!screen_data_interface.is_confirmed)
	{
		return;
	}
	screen_data_interface.send_byte(active_buffer[y][x]);
	if (++x == SCREEN_WIDTH)
	{
		x = 0;
		if (++y == SCREEN_HEIGHT)
		{
			y = 0;
			complete_picture();
		}
	}
}

void ScreenInterface::switch_buffer()
{
	uint8_t(*temp)[SCREEN_WIDTH] = active_buffer;
	active_buffer = buffer;
	buffer = temp;
}

inline void ScreenInterface::complete_picture()
{
	is_image_being_transmitted = 0;
}

void ScreenInterface::clear_screen(const Color color)
{
	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++){
			buffer[y][x] = color;
		}
	}
}

void ScreenInterface::draw_image(
		uint8_t start_x,
		uint8_t start_y,
		const uint8_t* data,
		const uint8_t width,
		const uint8_t height)
{
	const uint8_t max_y = height > SCREEN_HEIGHT ? SCREEN_HEIGHT : height;
	const uint8_t max_x = width > SCREEN_WIDTH ? SCREEN_WIDTH : width;
	for (uint8_t y = 0; y < max_y ; y++)
	{
		for (uint8_t x = 0; x < max_x; x++)
		{
			buffer[start_y + y][start_x + x] = data[y * width + x];
		}
	}
}

void ScreenInterface::draw_image(
		uint8_t start_x,
		uint8_t start_y,
		const ImageMono8x8& img,
		const Color color,
		const Color bg_color)
{
	const uint8_t max_y = start_y + img.height > SCREEN_HEIGHT - start_y ? SCREEN_HEIGHT : img.height;
	const uint8_t max_x = start_x + img.width > SCREEN_WIDTH ? SCREEN_WIDTH - start_x : img.width;
	for (uint8_t y = 0; y < max_y ; y++)
	{
		uint8_t *row = buffer[start_y + y];
		for (uint8_t x = 0; x < max_x; x++)
		{
			row[start_x + x] = (0b10000000 >> x) & img.data[y] ? color : bg_color;
		}
	}
}

void ScreenInterface::draw_number(
		uint8_t start_x,
		uint8_t start_y,
		float number,
		const Color color,
		const Color bg_color)
{
	uint8_t digits[5];
	uint16_t integer = (uint16_t) number;
	for (int8_t i = 0; i < 5; i++)
	{
		digits[i] = integer % 10;
		integer /= 10;
	}
	for (int8_t i = 4; i >= 0; i--)
	{
		const ImageMono8x8& img = CHARACTERS[digits[i] + DIGITS_OFFSET];
		draw_image(start_x, start_y, img, color, bg_color);
		start_x += (img.width + 1);
		if (start_x >= SCREEN_WIDTH)
		{
			break;
		}
	}
	draw_image(start_x, start_y, CHARACTERS[0], color, bg_color);
}
