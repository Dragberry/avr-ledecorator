#ifndef SANDBOXAPP_HPP_
#define SANDBOXAPP_HPP_

#include "../../dragberry/os.hpp"
#include "../../dragberry/os/display.hpp"

class SandboxApp :
        public Application,
        public Timer
{
private:
    enum Command : char
    {
        DRAW_POINT      = 'P',
        CLEAR_SCREEN    = 'C',
        SAVE_PICTURE    = 'S',
        LOAD_PICTURE    = 'L'
    };

    static const uint8_t LOAD_FRAME_SIZE = 8;

    static const uint8_t EEMEM STORED_FIELD[SCREEN_HEIGHT][SCREEN_WIDTH];

    uint8_t field[SCREEN_HEIGHT][SCREEN_WIDTH] = { };

    bool load_in_progress = false;
    uint8_t load_x = 0;
    uint8_t load_y = 0;
    uint8_t load_size = 0;

public:
    SandboxApp();

    ~SandboxApp();

    void run();

    void on_timer_event();
};

#endif
