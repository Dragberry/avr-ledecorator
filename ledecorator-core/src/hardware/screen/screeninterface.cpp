#include "screeninterface.hpp"

#include <avr/pgmspace.h>
#include "../../common/datatypeutils.h"

//#define SCRREN_DEBUG

ScreenInterface::ScreenInterface()
{
	#ifndef SCRREN_DEBUG
		UART::init(UART::BaudRate::B_1_250_000);
		UART::set_rx_handler(this);
	#endif
	Timers::T0::start(2, Timers::Prescaller::F_256, &screen_data_transmitter);
}

ScreenInterface::~ScreenInterface()
{
	UART::stop();
	Timers::T0::stop();
}

void ScreenInterface::update()
{
	if (!is_image_being_transmitted)
	{
		switch_buffer();
		start_picture();
	}
}

void ScreenInterface::on_uart_rx_event(uint8_t byte)
{
	is_byte_being_transmitted = false;
}

void ScreenInterface::start_picture()
{
	is_byte_being_transmitted = true;
	#ifndef SCRREN_DEBUG
		UART::send_byte(mask_command(CMD_DEFAULT));
	#endif
	is_image_being_transmitted = true;
}

void ScreenInterface::send_next_byte()
{
	if (is_byte_being_transmitted)
	{
		return;
	}
	is_byte_being_transmitted = true;
	#ifndef SCRREN_DEBUG
	UART::send_byte(active_buffer[y][x]);
	#endif
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
	is_image_being_transmitted = false;
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
		for (uint8_t x = start_x; x < max_x; x++)
		{
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
	draw(start_x, start_y, 0, 0, img->get_width(), img->get_height(),
			[bg_color, img](const uint8_t x, const uint8_t y) -> Color
			{
				const Color color = img->get_pixel(x, y);
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
