#ifndef DRAGBERRY_OS_HPP_
#define DRAGBERRY_OS_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "os/application.hpp"
#include "os/display.hpp"
#include "ble.hpp"
#include "../util/ringbuffer.hpp"

class Timer
{
public:
    virtual ~Timer();

    virtual void on_timer_event();
};

class System
{
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
    public:
        template <typename Out, typename In>
        static void exchange(Out&& out, In&& in)
        {
            if (BLE::is_connected())
            {
                char data[20] = { 0 };
                out(data);

                while (BLE::is_busy());
                BLE::state = BLE::State::PREPARING;
                uint8_t i = 0;
                while (!BLE::tx_frame.is_full())
                {
                    BLE::tx_frame.add(data[i++]);
                }
                BLE::state = BLE::State::READY;
                while (!BLE::start());

                while (BLE::state != BLE::State::IDLE);
                i = 0;
                while (!BLE::rx_frame.is_empty())
                {
                    data[i++] = BLE::rx_frame.poll();
                }
                in(data);
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
            frame_builder(BLE::tx_frame);
            BLE::state = BLE::State::READY;
            while (!BLE::start());
        }

        template <typename FrameBuilder>
        static void send(FrameBuilder&& frame_builder)
        {
            if (!BLE::is_busy())
            {
                BLE::state = BLE::State::PREPARING;
                frame_builder(BLE::tx_frame);
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
        uint8_t app_index = BLE::rx_frame.poll();
        uint8_t command = BLE::rx_frame.poll();
        if (app_index != 0)
        {
            if (app_index != current_app_index)
            {
                current_app->terminate();
                current_app_index = app_index;
            }
            else if (command == '0')
            {
                current_app->terminate();
            }
        }
    }
};

#endif
