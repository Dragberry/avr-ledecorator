#include <stdint.h>
#include "../screen.h"

uint8_t Screen::DefaultWorker::do_work()
{
	while (1)
	{
		uint8_t(*buffer)[SCREEN_WIDTH] = screen.buffer;
		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
			{
				uint8_t data = data_interface.get_data_byte();
				if (data & 0b01000000)
				{
					uint8_t command = data & 0b00111111;
					if (command < TOTAL_WORKERS)
					{
						return command;
					}
				}
				else
				{
					buffer[y][x] = data;
				}
			}
		}
		screen.switch_buffer();
	}
}
