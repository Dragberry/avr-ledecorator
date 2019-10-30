#include "processor.h"
#include "screen.h"

class FillScreenProcessor : public Processor
{
	uint8_t process(Screen* screen)
    {
        uint8_t data = read_byte();
        for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
        {
            for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
            {
                screen->buffer[y][x] = data;
            }
        }
        return read_command();
    }
};
