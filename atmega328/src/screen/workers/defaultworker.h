#ifndef DEFAULTWORKER_H_
#define DEFAULTWORKER_H_

#include <stdint.h>
#include "worker.h"

class DefaultWorker : public Worker
{
public:
	uint8_t do_work(Screen& screen)
	{
		const DataInterface* data_interface = screen.data_interface;
		while (1)
		{
			for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
			{
				for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
				{
					uint8_t data = data_interface->get_data_byte();
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
						screen.buffer[y][x] = data;
					}
				}
			}
			screen.switch_buffer();
		}
	}

};



#endif
