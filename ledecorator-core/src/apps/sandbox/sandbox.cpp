#include "sandboxapp.hpp"

const uint8_t EEMEM SandboxApp::STORED_FIELD[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};

SandboxApp::SandboxApp()
{
    eeprom_read_block((void*) &field, (const void*) &STORED_FIELD, sizeof(STORED_FIELD));
    this->time_to_live = 30;
}

SandboxApp::~SandboxApp()
{
}

void SandboxApp::on_timer_event()
{
    increment_time();
}

void SandboxApp::run()
{
    dragberry::os::display::clear_screen(BLACK);
    dragberry::os::display::update_assured();
    System::register_timer(this, 100);
    while (is_going_on())
    {

        System::io::exchange(
            [&](char* frame) -> void
            {
                frame[1] = System::APP_SANDBOX;
                System::io::decompose(time, 2);
            },
            [&](char* frame) -> void
            {
                switch (frame[3])
                {
                case Command::DRAW_POINT:
                    {
                        uint8_t x = frame[4];
                        uint8_t y = frame[5];
                        if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
                        {
                            field[y][x] = frame[6];
                        }
                    }
                    break;
                case Command::CLEAR_SCREEN:
                    {
                        Color color = frame[4];
                        for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
                        {
                            for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
                            {
                                field[y][x] = color;
                            }
                        }
                    }
                    break;
                case Command::SAVE_PICTURE:
                    eeprom_update_block((const void*) &field, (void*) &STORED_FIELD, sizeof(STORED_FIELD));
                    break;
                default:
                    break;
                }
            }
        );
        for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
        {
            for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
            {
                dragberry::os::display::set_pixel(y, x, field[y][x]);
            }
        }
        dragberry::os::display::update_assured();
    }
    System::deregister_timer();
}
