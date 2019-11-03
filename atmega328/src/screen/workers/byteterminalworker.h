#ifndef BYTETERMINALWORKER_H_
#define BYTETERMINALWORKER_H_

#include "worker.h"
#include "../interfaces/datainterface.h"

class ByteTerminalWorker : public Worker
{
public:
	uint8_t do_work(Screen& screen)
	{
		while (1)
		{
			const DataInterface* data_interface = screen.data_interface;
			uint8_t cr = 0;
			uint8_t lf = 0;
			uint8_t** buffer = screen.buffer;
			for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
			{
				uint8_t data = (cr && lf) ? BLACK : data_interface->get_data_byte();
				if (data & 0b01000000)
				{
					uint8_t command = data & 0b00111111;
					if (command < TOTAL_WORKERS)
					{
						return command;
					}
				}
				buffer[7][x] = data & (1<<0) ? data : BLACK;
				buffer[6][x] = data & (1<<1) ? data : BLACK;
				buffer[5][x] = data & (1<<2) ? data : BLACK;
				buffer[4][x] = data & (1<<3) ? data : BLACK;
				buffer[3][x] = data & (1<<4) ? data : BLACK;
				buffer[2][x] = data & (1<<5) ? data : BLACK;
				buffer[1][x] = data & (1<<6) ? data : BLACK;
				buffer[0][x] = data & (1<<7) ? data : BLACK;

				if (!lf)
				{
					if (data == CR)
					{
						cr = data;
					}
					else if (data == LF && cr)
					{
						lf = data;
					}
					else
					{
						cr = 0;
					}
				}
			}


			for (uint8_t y = 8; y < SCREEN_HEIGHT; y++)
			{
				uint8_t* row = buffer[y];
				for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
				{
					row[x] = BLACK;
				}
			}
			screen.switch_buffer();
		}
	}

};

#endif
