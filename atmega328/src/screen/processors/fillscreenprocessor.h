#include "buffer.h"
#include "processor.h"

class FillScreenProcessor : public Processor
{
	uint8_t process(Buffer* buffer)
    {
        uint8_t data = read_byte();
        for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
        {
            for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
            {
                buffer->buffer[y][x] = data;
            }
        }
        return read_command();
    }
};
