#ifndef SCREENINTERFACE_HPP_
#define SCREENINTERFACE_HPP_

#include <avr/io.h>
#include <stdint.h>

#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"
#include "lib/screen/colors.h"
#include "lib/screen/commands.h"
#include "lib/screen/definitions.h"

#include "../../data/image.h"
#include "../../data/imagemono8x8.h"

class ScreenInterface : public UART::RxHandler
{
private:
	uint8_t buffer_1[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t buffer_2[SCREEN_HEIGHT][SCREEN_WIDTH];

	uint8_t y = 0;
	uint8_t x = 0;

	volatile uint8_t is_byte_being_transmitted = 0;

	class ScreenDataTransmitter : public Timers::Handler
	{
	public:
		ScreenInterface& screen_interace;

		ScreenDataTransmitter(ScreenInterface& screen_interace) : screen_interace(screen_interace) {}

		void handle()
		{
			if (screen_interace.is_image_being_transmitted)
			{
				screen_interace.send_next_byte();
			}
		}

	} screen_data_transmitter = ScreenDataTransmitter(*this);

public:
	enum Align : uint8_t
	{
		LEFT, RIGHT
	};

	volatile uint8_t is_image_being_transmitted = 0;

	uint8_t(*active_buffer)[SCREEN_WIDTH] = buffer_1;
	uint8_t(*buffer)[SCREEN_WIDTH] = buffer_2;

	ScreenInterface();

	~ScreenInterface();

	void handle_rx(const uint8_t byte);

	void start_picture();

	void send_next_byte();

	void switch_buffer();

	void complete_picture();

	void clear_screen(const Color color);

	void draw_line_vertical(
			const uint8_t start_x,
			uint8_t start_y,
			const uint8_t length,
			const Color color);

	void draw_line_horizontal(
			uint8_t start_x,
			const uint8_t start_y,
			const uint8_t length,
			const Color color);

	void draw_area(
			const uint8_t start_x,
			uint8_t start_y,
			const uint8_t width,
			const uint8_t height,
			const Color color);

	void draw_image(
			uint8_t start_x,
			uint8_t start_y,
			const Image* img,
			const Color bg_color);

	void draw_image(
			uint8_t start_x,
			uint8_t start_y,
			const int8_t offset_x,
			const int8_t offset_y,
			const ImageMono8x8* img,
			const Color color,
			const Color bg_color);

//	void draw_histogram(
//			uint8_t start_x,
//			uint8_t start_y,
//			const uint8_t width,
//			const uint8_t height,
//			const int16_t* data_set,
//			const uint8_t data_set_size,
//			const Color color,
//			const Color bg_color);

	template<typename PixelProvider>
	void draw(
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
			uint8_t(*row) = buffer[start_y + real_y];
			for (int8_t x = 0; x < max_width; x++)
			{
				int8_t real_x = x + offset_x;
				if (real_x < 0 || real_x >= width)
				{
					continue;
				}
				row[start_x + real_x] = get_pixel(x, y);
			}
		}
	}


};

#endif
