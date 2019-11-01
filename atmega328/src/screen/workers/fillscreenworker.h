#include "../screen.h"
#include "../workers/worker.h"

class FillScreenWorker : public Worker
{
	uint8_t do_work(Screen* screen)
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
