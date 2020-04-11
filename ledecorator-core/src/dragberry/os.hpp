#ifndef DRAGBERRY_OS_HPP_
#define DRAGBERRY_OS_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "os/application.hpp"
#include "os/display.hpp"
#include "ble.hpp"
#include "../util/ringbuffer.hpp"
#include "lib/avr/hardware/spi.hpp"

class Timer
{
public:
    virtual ~Timer();

    virtual void on_timer_event();
};

class System
{
public:
    static const char APP_IDLE = 'I';
    static const char APP_SNAKE = 'S';
    static const char APP_WEATHER = 'W';

    static const char COMMAND_TERMINATE = 'T';

private:
    static volatile uint16_t time;

    static uint16_t period;

    static Timer *timer;

public:
    static void init();

    static void register_timer(Timer* timer, uint8_t period);

    static void deregister_timer(Timer* timer);

    static void on_system_timer_event();

    class io
    {
    private:
        static const char FRAME_START = 1;

        static const char FRAME_END = 4;

    public:
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
                out(BLE::tx_buffer);

                BLE::state = BLE::State::READY;
                while (!BLE::start());

                while (BLE::state != BLE::State::IDLE);
                if (BLE::rx_buffer[0] == FRAME_START && BLE::rx_buffer[19] == FRAME_END)
                {
                    in(BLE::rx_buffer);
                }
            }
        }

    };

    class out
    {
    public:
        template <typename FrameBuilder>
        static void send_assured(FrameBuilder&& frame_builder)
        {
            while (BLE::is_busy());
            BLE::state = BLE::State::PREPARING;
//            frame_builder(BLE::tx_frame);
            BLE::state = BLE::State::READY;
            while (!BLE::start());
        }

        template <typename FrameBuilder>
        static void send(FrameBuilder&& frame_builder)
        {
            if (!BLE::is_busy())
            {
                BLE::state = BLE::State::PREPARING;
//                frame_builder(BLE::tx_frame);
                BLE::state = BLE::State::READY;
            }
        }
    };

private:
    static volatile uint8_t current_app_index;

    static Application* current_app;

public:
    template<typename Execution>
    static void run(Execution &&execution)
    {
        execution();
    }

    static void set_app(Application* app)
    {
        current_app = app;
    }

    static void process_event()
    {
        uint8_t app_index = BLE::tx_buffer[0];
        uint8_t command = BLE::tx_buffer[1];
        if (app_index != APP_IDLE)
        {
            if (app_index != current_app_index)
            {
                current_app->terminate();
                current_app_index = app_index;
            }
            else if (command == COMMAND_TERMINATE)
            {
                current_app->terminate();
            }
        }
    }
};

#endif
