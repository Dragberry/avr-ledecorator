#ifndef DEFAULTSCREENPROCESSOR_H_
#define DEFAULTSCREENPROCESSOR_H_

#include "buffer.h"
#include "processor.h"
#include "../color.h"

class DefaultScreenProcessor : public Processor
{
private:
	uint8_t state = 0;

public:
	uint8_t process(Buffer* buffer)
	{
		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
			{
				uint8_t color;
				if (state > 224)
				{
					color = RED;
				}
				else if(state > 192)
				{
					color = YELLOW;
				}
				else if(state > 160)
				{
					color = GREEN;
				}
				else if(state > 128)
				{
					color = CYAN;
				}
				else if(state > 96)
				{
					color = BLUE;
				}
				else if(state > 64)
				{
					color = MAGENTA;
				}
				else if(state > 32)
				{
					color = WHITE;
				}
				else
				{
					color = BLACK;
				}


				buffer->buffer[y][x] = color;
			}
		}
		state++;
		buffer->switch_buffer();
		return 0;
	}
};

#endif
