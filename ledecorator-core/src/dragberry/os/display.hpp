#ifndef DRAGBERRY_DISPLAY_HPP_
#define DRAGBERRY_DISPLAY_HPP_

#include <stdint.h>
#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"
#include "lib/screen/colors.h"
#include "lib/screen/commands.h"
#include "lib/screen/definitions.h"
#include "../../data/image.h"
#include "../../data/imagemono8x8.h"

#define FPS_DEBUG

namespace dragberry
{
	namespace os
	{
		class display
		{
		public:
			struct Buffers
			{
			private:
				uint8_t buffer_1[SCREEN_HEIGHT][SCREEN_WIDTH];
				uint8_t buffer_2[SCREEN_HEIGHT][SCREEN_WIDTH];

			public:
				uint8_t(*active_buffer)[SCREEN_WIDTH] = buffer_1;
				uint8_t(*buffer)[SCREEN_WIDTH] = buffer_2;

				void swap();
			};

		public:
			class Transmitter :
					public Timers::T0::Handler,
					public UART::RxHandler
			{
			private:
				volatile uint8_t y = 0;
				volatile uint8_t x = 0;

			public:
				volatile bool is_connected = false;
				volatile bool is_busy = false;

				volatile enum State
				{
					IDLE,
					FRAME_START,
					TRANSMIT,
					FRAME_END
				} state = IDLE;

			public:
				Transmitter();

			public:
				void enable();

				void disable();

				void start_new_frame();

			public:
				void on_timer0_event();
			private:
				void send_byte(const uint8_t byte);

			public:
				void on_uart_rx_event(const uint8_t byte);

			};

		public:
			static void connect();

			static void disconnect();

			static void update_requsted();

			static void update_assured();

			static void set_pixel(uint8_t y, uint8_t x, Color color);

			static void clear_screen(const Color color);

			static void draw_line_vertical(
					const uint8_t start_x,
					uint8_t start_y,
					const uint8_t length,
					const Color color);

			static void draw_line_horizontal(
					uint8_t start_x,
					const uint8_t start_y,
					const uint8_t length,
					const Color color);

			static void draw_area(
					const uint8_t start_x,
					uint8_t start_y,
					const uint8_t width,
					const uint8_t height,
					const Color color);

			static void draw_image(
					uint8_t start_x,
					uint8_t start_y,
					const Image* img,
					const Color bg_color);

			static void draw_image(
					uint8_t start_x,
					uint8_t start_y,
					const int8_t offset_x,
					const int8_t offset_y,
					const ImageMono8x8* img,
					const Color color,
					const Color bg_color);

			template<typename PixelProvider>
			static void draw(
					int8_t start_x,
					int8_t start_y,
					const int8_t offset_x,
					const int8_t offset_y,
					const int8_t width,
					const int8_t height,
					const PixelProvider&& get_pixel)
			{
				int8_t max_height = SCREEN_HEIGHT - start_y;
				if (max_height > height)
				{
					max_height = height;
				}
				int8_t max_width = SCREEN_WIDTH - start_x;
				if (max_width > width)
				{
					max_width = width;
				}
				for (int8_t y = 0; y < max_height; y++)
				{
					int8_t real_y = y + offset_y;
					if (real_y < 0 || real_y >= height)
					{
						continue;
					}
					uint8_t row_idx = start_y + real_y;
					for (int8_t x = 0; x < max_width; x++)
					{
						int8_t real_x = x + offset_x;
						if (real_x < 0 || real_x >= width)
						{
							continue;
						}
						set_pixel(row_idx, start_x + real_x, get_pixel(x, y));
					}
				}
			}
		};
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

#endif
