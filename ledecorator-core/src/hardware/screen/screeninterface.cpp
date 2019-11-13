#include "../../common/datatypeutils.h"
#include "../../data/characters.h"
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

void ScreenInterface::clear_area(
		uint8_t start_x,
		uint8_t start_y,
		const uint8_t width,
		const uint8_t height,
		Color color)
{
	uint8_t max_y = start_y + height;
	if (max_y > SCREEN_HEIGHT)
	{
		max_y = SCREEN_HEIGHT;
	}
	uint8_t max_x = start_x + width;
	if (max_x > SCREEN_WIDTH)
	{
		max_x = SCREEN_WIDTH;
	}
	while (start_y < max_y)
	{
		for (uint8_t x = start_x; x < max_x; x++){
			buffer[start_y][x] = color;
		}
		start_y++;
	}
}

void ScreenInterface::draw_image(
		uint8_t start_x,
		uint8_t start_y,
		const uint8_t* data,
		const uint8_t width,
		const uint8_t height)
{
	uint8_t max_y = start_y + height;
	if (max_y > SCREEN_HEIGHT)
	{
		max_y = SCREEN_HEIGHT;
	}
	uint8_t max_x = start_x + width;
	if (max_x > SCREEN_WIDTH)
	{
		max_x = SCREEN_WIDTH;
	}
	for (uint8_t y = 0, row_offset = 0; y < max_y ; y++, row_offset += width)
	{
		for (uint8_t x = 0; x < max_x; x++)
		{
			buffer[start_y + y][start_x + x] = data[row_offset + x];
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
	uint8_t height = SCREEN_HEIGHT - start_y;
	if (height > img.height)
	{
		height = img.height;
	}
	uint8_t width = SCREEN_WIDTH - start_x;
	if (width > img.width)
	{
		width = img.width;
	}
	for (uint8_t y = 0; y < height ; y++)
	{
		uint8_t *row = buffer[start_y + y];
		for (uint8_t x = 0; x < width; x++)
		{
			row[start_x + x] = (0b10000000 >> x) & img.data[y] ? color : bg_color;
		}
	}
}

void ScreenInterface::draw_number(
		uint8_t start_x,
		uint8_t start_y,
		float number,
		const bool sign,
		const uint8_t pr_int,
		const uint8_t pr_float,
		const Color color,
		const Color bg_color)
{

	char data[sign ? 1 : 0 + pr_int + 1 + pr_float];
	if (sign)
	{
		data[0] = number > 0 ? '+' : '-';
	}
	float_to_string(data + (sign ? 1 : 0), number, pr_int, pr_float);
	for (uint8_t i = 0; i < 5; i++)
	{
		const ImageMono8x8& img = CHARACTERS[data[i] + DIGITS_OFFSET - '0'];
		draw_image(start_x, start_y, img, color, bg_color);
		start_x += (img.width + 1);
		if (start_x >= SCREEN_WIDTH)
		{
			break;
		}
	}

}

