#ifndef SCREENINTERFACE_H_
#define SCREENINTERFACE_H_

#include <avr/io.h>
#include <stdint.h>
#include "../../rgb-32x16-screen/lib/screen/commands.h"
#include "../../rgb-32x16-screen/lib/screen/definitions.h"

class ScreenInterface
{
	class DataInterface
	{
	private:
		volatile uint8_t is_last_byte_confirmed = 1;
	public:
		inline void send_byte(const uint8_t data)
		{
			is_last_byte_confirmed = 0;
			UDR0 = data;
		}

		inline uint8_t byte_confirmed()
		{
			is_last_byte_confirmed = 1;
			return UDR0;
		}
	};

private:
	DataInterface data_interface;

	volatile uint8_t is_image_being_transmitted = 0;
	uint8_t buffer_1[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t buffer_2[SCREEN_HEIGHT][SCREEN_WIDTH];
	uint8_t(*active_buffer)[SCREEN_WIDTH] = buffer_1;
	uint8_t(*buffer)[SCREEN_WIDTH] = buffer_2;

	uint8_t y;
	uint8_t x;

public:
	inline void swith_buffer()
	{
		uint8_t(*temp)[SCREEN_WIDTH] = active_buffer;
		active_buffer = buffer;
		buffer = temp;
	}

	inline void start_picture()
	{
		data_interface.send_byte(COMMAND_MASK & CMD_DEFAULT);
		is_image_being_transmitted = 1;
	}

	inline void complete_picture()
	{
		is_image_being_transmitted = 0;
	}

	inline void send()
	{
		data_interface.send_byte(active_buffer[y][x]);
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
};

#endif
