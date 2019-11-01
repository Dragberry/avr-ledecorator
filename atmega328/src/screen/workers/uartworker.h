#ifndef UARTWORKER_H_
#define UARTWORKER_H_

#include "../colors.h"
#include "../workers/worker.h"

class UartWorker : public Worker
{

private:
	uint8_t y = 0;
	uint8_t x = 0;

public:
	uint8_t do_work(Screen* screen)
	{
		uint8_t skip = 0;
		uint8_t** buffer = screen->buffer;
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
		{
			uint8_t data = read_byte();
			if (is_command(data))
			{
				uint8_t command = data & 0b00111111;
				if (command < 3)
				{
					return command;
				}
				else {
					break;
				}
			}


			data = skip ? BLACK : read_byte();
			buffer[0][x] = data & (1<<0) ? data : BLACK;
			buffer[1][x] = data & (1<<1) ? data : BLACK;
			buffer[2][x] = data & (1<<2) ? data : BLACK;
			buffer[3][x] = data & (1<<3) ? data : BLACK;
			buffer[4][x] = data & (1<<4) ? data : BLACK;
			buffer[5][x] = data & (1<<5) ? data : BLACK;
			buffer[6][x] = data & (1<<6) ? data : BLACK;
			buffer[7][x] = data & (1<<7) ? data : BLACK;
			if (data == LF)
			{
				skip = 1;
			}
		}


		for (uint8_t y = 8; y < SCREEN_HEIGHT; y++)
		{
			uint8_t* row = screen->buffer[y];
			for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
			{
				row[x] = BLACK;
			}
		}
		screen->switch_buffer();
		return 0;
	}
};

#endif
