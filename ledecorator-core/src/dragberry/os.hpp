#ifndef DRAGBERRY_OS_HPP_
#define DRAGBERRY_OS_HPP_

#include <avr/eeprom.h>
#include "lib/avr/hardware/timers.hpp"
#include "ble.hpp"
#include "os/application.hpp"
#include "os/display.hpp"
#include "../util/ringbuffer.hpp"

class Timer
{
public:
    virtual ~Timer();

    virtual void on_timer_event();
};

struct Execution
{
    char code;
    void (*exec)();
};

class System
{
public:
    static const char APP_IDLE = 'I';
    static const char APP_SNAKE = 'S';
    static const char APP_WEATHER = 'W';
    static const char APP_CLOCK = 'C';
    static const char APP_LIFE = 'L';
    static const char APP_SANDBOX = 'B';

    static const char COMMAND_INFINITE = 'I';
    static const char COMMAND_FINITE = 'F';
    static const char COMMAND_RESTART = 'R';
    static const char COMMAND_LOAD = 'L';
    static const char COMMAND_SAVE = 'S';
    static const char COMMAND_EMPTY = 'E';

    static const uint8_t TIMER_FREQ = 100;

private:
    static volatile uint16_t time;

    static uint16_t period;

    static Timer *timer;

    static volatile uint8_t curr_app_code;

    static volatile uint8_t next_app_code;

    static const uint8_t NUMBER_OF_PROGRAMMS;

    static const uint8_t EEMEM NUMBER_OF_STORED_PROGRAMMS;

    static const uint8_t EEMEM STORED_PROGRAMMS[];

    static uint8_t number_of_loaded_programms;

    static Execution loaded_programms[];

    static bool load_requested;

    static void request_load();

    static void do_load();

    static void do_save();

public:
    static Application* current_app;

    static void register_timer(Timer* timer, uint16_t period);

    static void deregister_timer();

    static void on_system_timer_event();

    static void init();

    static void run();

    static void load();

    static void process_event();

    class io
    {
    private:
        static const char FRAME_START = 1;

        static const char FRAME_END = 4;

    public:
        static void decompose(const uint16_t& value, const uint8_t idx);

        static void decompose(const int32_t& value, const uint8_t idx);

        static void compose(uint16_t& value, const uint8_t idx);

        template <typename Out, typename In>
        static void exchange(Out&& out, In&& in)
        {
            if (BLE::is_connected())
            {
                while (BLE::is_busy());
                BLE::state = BLE::State::PREPARING;

                uint8_t i = 0;
                while (i < 20)
                {
                    BLE::tx_buffer[i] = '\0';
                    BLE::rx_buffer[i] = '\0';
                    i++;
                }
                BLE::tx_buffer[0] = FRAME_START;
                BLE::tx_buffer[19] = FRAME_END;
                if (load_requested)
                {
                    load_requested = false;
                    do_load();
                }
                else
                {
                    out(BLE::tx_buffer);
                }
                BLE::state = BLE::State::READY;
                while (!BLE::start());

                while (BLE::state != BLE::State::IDLE);
                if (BLE::rx_buffer[0] == FRAME_START && BLE::rx_buffer[19] == FRAME_END)
                {
                    if (BLE::rx_buffer[2] != COMMAND_LOAD && BLE::rx_buffer[2] != COMMAND_SAVE)
                    {
                        in(BLE::rx_buffer);
                    }
                }
            }
        }

    };

};

#endif
