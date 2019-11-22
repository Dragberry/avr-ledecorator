#include "screeninterface.h"
#include <avr/pgmspace.h>
#include <math.h>
#include "../../common/datatypeutils.h"

#define INT16_MAX 0x7fff
#define INT16_MIN (-INT16_MAX - 1)


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

void ScreenInterface::draw_line_vertical(
		const uint8_t start_x,
		uint8_t start_y,
		const uint8_t length,
		const Color color)
{
	if (start_x > SCREEN_WIDTH)
	{
		return;
	}
	uint8_t max_y = start_y + length;
	if (max_y > SCREEN_HEIGHT)
	{
		max_y = SCREEN_HEIGHT;
	}
	while (start_y < max_y)
	{
		buffer[start_y++][start_x] = color;
	}
}

void ScreenInterface::draw_line_horizontal(
		uint8_t start_x,
		const uint8_t start_y,
		const uint8_t length,
		const Color color)
{
	if (start_y > SCREEN_HEIGHT)
	{
		return;
	}
	uint8_t max_x = start_x + length;
	if (max_x > SCREEN_WIDTH)
	{
		max_x = SCREEN_WIDTH;
	}
	while (start_x < max_x)
	{
		buffer[start_y][start_x++] = color;
	}
}

void ScreenInterface::draw_area(
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
		const Image* img,
		const Color bg_color)
{
	const uint8_t img_width = pgm_read_byte(&(img->width));
	draw(start_x, start_y, 0, 0, img_width, pgm_read_byte(&(img->height)),
			[bg_color, img, img_width](const uint8_t x, const uint8_t y) -> Color
			{
				const uint8_t* data = (uint8_t*) pgm_read_ptr(&(img->data));
				const Color color = pgm_read_byte(&(data[y * img_width + x]));
				if (bg_color != BLACK && color == BLACK)
				{
					return bg_color;
				}
				return color;
			});
}

void ScreenInterface::draw_image(
		uint8_t start_x,
		uint8_t start_y,
		const int8_t offset_x,
		const int8_t offset_y,
		const ImageMono8x8* img,
		const Color color,
		const Color bg_color)
{
	draw(start_x, start_y, offset_x, offset_y, img->get_width(), img->get_height(),

			[img, color, bg_color](const uint8_t x, const uint8_t y) -> Color
			{
				return img->get_bit(x, y) ? color : bg_color;
			});
}

void ScreenInterface::draw_string(
		const char* string,
		const uint8_t string_size,
		uint8_t start_x,
		uint8_t start_y,
		const int8_t offset_x,
		const int8_t offset_y,
		const uint8_t width,
		const uint8_t height,
		const Color color,
		const Color bg_color)
{
	int8_t passed_width = offset_x;
	if (offset_x > 0)
	{
		draw_area(start_x, start_y, passed_width, height, bg_color);
		if (passed_width >= width)
		{
			return;
		}
		start_x += offset_x;
	}
	int8_t passed_string_width = 0;
	for (uint8_t i = 0; i < string_size; i++)
	{
		const ImageMono8x8* img = ImageMono8x8::for_character(string[i]);
		const uint8_t img_width = img->get_width();
		passed_string_width += img_width ;
		passed_width += img_width ;
		if (passed_width < -1)
		{
			passed_width++;
			continue;
		}
		else
		{
			int8_t char_offset_x = passed_width - img_width ;
			if (char_offset_x > 0)
			{
				char_offset_x = 0;
			}
			draw_image(start_x, start_y, char_offset_x, 0, img, color, bg_color);
			start_x += (img_width  + char_offset_x);
		}
		if (passed_width >= width)
		{
			break;
		}
		passed_width++;
		if (passed_width < 0)
		{
			continue;
		}
		else if (passed_width >= width)
		{
			break;
		}
		else
		{
			draw_line_vertical(start_x++, start_y, height, bg_color);
		}
	}
	if (passed_width < width)
	{
		draw_area(start_x, start_y, width - passed_width, height, bg_color);
	}
}

//void ScreenInterface::draw_histogram(
//			uint8_t start_x,
//			uint8_t start_y,
//			const uint8_t width,
//			const uint8_t height,
//			const int16_t* data_set,
//			const uint8_t data_set_size,
//			const Color color,
//			const Color bg_color)
//{
//	// 720, 744, 761, 738, 739, 741
//	int16_t max = INT16_MIN; // 761
//	int16_t min = INT16_MAX; // 720
//	uint8_t i = 0;
////	while (i < data_set_size)
////	{
////		int16_t current = data_set[i];
////		if (current > max)
////		{
////			max = current;
////		}
////		if (current < min)
////		{
////			min = current;
////		}
////	}
////	int16_t difference = max - min; 	// 41
////	int16_t step = difference / height;	// 41 / 8 = 5
////	i = 0;
////	uint8_t values[data_set_size];
////	while (i < data_set_size)
////	{
////		int16_t temp = data_set[i] - min;
////		// 720 - 720 = 0
////		// 720 - 744 = 24
////		// 720 - 761 = 31
////		uint8_t value = (temp / step) + 1;
////		// 0 / 5 + 1= 0
////		// 24 / 5 + 1 = 5
////		// 31 / 5 = 7
////		values[i++] = value;
////	}
//	uint8_t values[6] = {0,1,2,3,4,5};
//	i = 0;
//	draw_area(start_x, start_y, 1, height, bg_color);
//	while ()
//	{
//
//	}
//
//	start_x++;
//	while (i < data_set_size)
//	{
//		for (uint8_t y = 0; y < height; y++)
//		{
//			if (values[i] > (height - y - 1))
//			{
//				buffer[start_y + y][start_x] = RED;
//				buffer[start_y + y][start_x + 1] = RED;
//			}
//		}
//		start_x += 4;
//		i++;
//	}
//}
