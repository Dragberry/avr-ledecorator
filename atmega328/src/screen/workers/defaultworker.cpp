#include <stdint.h>
#include "../screeninterface.hpp"

void ScreenInterface::DefaultWorker::work_with_command(const uint8_t command)
{
	y = 0;
	x = 0;
}

void ScreenInterface::DefaultWorker::work_with_byte(const uint8_t byte)
{
	screen_interface.buffer[y][x] = byte;
	if (++x == SCREEN_WIDTH)
	{
		x = 0;
		if (++y == SCREEN_HEIGHT)
		{
			y = 0;
			screen_interface.switch_accumulator();
		}
	}
}
