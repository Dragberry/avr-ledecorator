#include <stdint.h>
#include "lib/screen/commands.h"
#include "../screeninterface.hpp"

void ScreenInterface::ByteTerminalWorker::work_with_command(const uint8_t command)
{
	x = 0;
}

void ScreenInterface::ByteTerminalWorker::work_with_byte(const uint8_t byte)
{

	screen_interface.buffer[7][x] = byte & (1<<0) ? byte : BLACK;
	screen_interface.buffer[6][x] = byte & (1<<1) ? byte : BLACK;
	screen_interface.buffer[5][x] = byte & (1<<2) ? byte : BLACK;
	screen_interface.buffer[4][x] = byte & (1<<3) ? byte : BLACK;
	screen_interface.buffer[3][x] = byte & (1<<4) ? byte : BLACK;
	screen_interface.buffer[2][x] = byte & (1<<5) ? byte : BLACK;
	screen_interface.buffer[1][x] = byte & (1<<6) ? byte : BLACK;
	screen_interface.buffer[0][x] = byte & (1<<7) ? byte : BLACK;

	if (!lf)
	{
		if (byte == CR)
		{
			cr = byte;
		}
		else if (byte == LF && cr)
		{
			lf = byte;
		}
		else
		{
			cr = 0;
		}
	}
	for (uint8_t y = 8; y < SCREEN_HEIGHT; y++)
	{
		uint8_t* row = screen_interface.buffer[y];
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			row[x] = BLACK;
		}
	}
//	screen_interface.switch_buffer();
}

