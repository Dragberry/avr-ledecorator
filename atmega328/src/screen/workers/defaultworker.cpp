#include <stdint.h>
#include "../screeninterface.hpp"

#include <avr/io.h>

void ScreenInterface::DefaultWorker::work_with_command(const uint8_t command)
{
	switch (command)
	{
	case CMD_DEFAULT_FRAME_START:
		y = 0;
		x = 0;
		break;
	case CMD_DEFAULT_FRAME_END:
		screen_interface.switch_accumulator();
		break;
	default:
		break;
	}
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
		}
	}
}
