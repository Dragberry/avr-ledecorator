#include "screeninterface.h"

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
		const uint8_t* data,
		const uint8_t width,
		const uint8_t height,
		const Color color,
		const Color bg_color)
{
	const uint8_t max_y = height > SCREEN_HEIGHT ? SCREEN_HEIGHT : height;
	const uint8_t max_x = width > SCREEN_WIDTH ? SCREEN_WIDTH : width;
	for (uint8_t y = 0; y < max_y ; y++)
	{
		for (uint8_t x = 0; x < max_x; x++)
		{
			buffer[start_y + y][start_x + x] = data[y * width + x] ? color : bg_color;
		}
	}
}
