#ifndef DEFAULTSCREENWORKER_H_
#define DEFAULTSCREENWORKER_H_

#include "../colors.h"
#include "../screen.h"
#include "worker.h"

#define ORANGE 0b00000111

class DefaultScreenWorker : public Worker
{
private:
	uint8_t state = 0;

	uint8_t pumpkin_color = ORANGE;
	uint8_t fire_color = YELLOW;

public:
	uint8_t do_work(Screen* screen)
	{
		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
			{
				screen->buffer[y][x] = BLUE;
			}
		}
		// 0...31
		// 15
		uint8_t** buf = screen->buffer;

		uint8_t row_index = 1;
		uint8_t* row = buf[row_index++];
		row[16]=GREEN;

		row = buf[row_index++];
		row[15]=GREEN;

		row = buf[row_index++];
		row[11]=pumpkin_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=pumpkin_color;

		row = buf[row_index++];
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=pumpkin_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=pumpkin_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;

		row = buf[row_index++];
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=pumpkin_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=pumpkin_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;

		row = buf[row_index++];
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=fire_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=fire_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;

		row = buf[row_index++];
		row[7]=pumpkin_color;
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=fire_color;
		row[12]=fire_color;
		row[13]=fire_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=fire_color;
		row[18]=fire_color;
		row[19]=fire_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;
		row[23]=pumpkin_color;

		row = buf[row_index++];
		row[7]=pumpkin_color;
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=pumpkin_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=pumpkin_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;
		row[23]=pumpkin_color;

		row = buf[row_index++];
		row[7]=pumpkin_color;
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=fire_color;
		row[11]=pumpkin_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=pumpkin_color;
		row[20]=fire_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;
		row[23]=pumpkin_color;

		row = buf[row_index++];
		row[7]=pumpkin_color;
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=fire_color;
		row[11]=pumpkin_color;
		row[12]=fire_color;
		row[13]=fire_color;
		row[14]=pumpkin_color;
		row[15]=fire_color;
		row[16]=fire_color;
		row[17]=fire_color;
		row[18]=pumpkin_color;
		row[19]=fire_color;
		row[20]=fire_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;
		row[23]=pumpkin_color;

		row = buf[row_index++];
		row[7]=pumpkin_color;
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=fire_color;
		row[11]=fire_color;
		row[12]=fire_color;
		row[13]=fire_color;
		row[14]=fire_color;
		row[15]=fire_color;
		row[16]=fire_color;
		row[17]=fire_color;
		row[18]=fire_color;
		row[19]=fire_color;
		row[20]=fire_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;
		row[23]=pumpkin_color;

		row = buf[row_index++];
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=fire_color;
		row[12]=pumpkin_color;
		row[13]=fire_color;
		row[14]=fire_color;
		row[15]=fire_color;
		row[16]=fire_color;
		row[17]=pumpkin_color;
		row[18]=fire_color;
		row[19]=fire_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;

		row = buf[row_index++];
		row[8]=pumpkin_color;
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=pumpkin_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=pumpkin_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;
		row[22]=pumpkin_color;

		row = buf[row_index++];
		row[9]=pumpkin_color;
		row[10]=pumpkin_color;
		row[11]=pumpkin_color;
		row[12]=pumpkin_color;
		row[13]=pumpkin_color;
		row[14]=pumpkin_color;
		row[15]=pumpkin_color;
		row[16]=pumpkin_color;
		row[17]=pumpkin_color;
		row[18]=pumpkin_color;
		row[19]=pumpkin_color;
		row[20]=pumpkin_color;
		row[21]=pumpkin_color;

//		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
//		{
//			for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
//			{
//				// 512 / 32
//				uint8_t xx = state % SCREEN_WIDTH;
//				// 512 / 32
//				uint8_t yy = state / SCREEN_WIDTH;
//
//				screen->buffer[y][x] = BLUE;
//				screen->buffer[yy][xx] = RED;
//
//
//			}
//		}
//		if (++state == 512)
//		{
//			state = 0;
//		}

		fire_color = state++ > 31 ? YELLOW : BLACK;
		if (state == 64)
		{
			state = 0;
		}
		screen->switch_buffer();
		return 0;
	}
};

#endif
