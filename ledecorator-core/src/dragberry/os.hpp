#ifndef DRAGBERRY_OS_HPP_
#define DRAGBERRY_OS_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "os/display.hpp"
#include "ble.hpp"
#include "../util/ringbuffer.hpp"


class Timer
{
public:
    virtual ~Timer();

    virtual void on_timer_event();
};

class IOHandler
{
public:
    virtual ~IOHandler();

    virtual void on_input_event(RingBuffer<uint8_t, 20>* frame);
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

    class out
    {
    public:
        template <typename FrameBuilder>
        static void send_assured(FrameBuilder&& frame_builder)
        {
            while (BLE::is_busy());
            BLE::state = BLE::State::PREPARING;
            frame_builder(BLE::tx_buffer);
            BLE::state = BLE::State::READY;
            while (!BLE::start());
        }

        template <typename FrameBuilder>
        static void send(FrameBuilder&& frame_builder)
        {
            if (!BLE::is_busy())
            {
                BLE::state = BLE::State::PREPARING;
                frame_builder(BLE::tx_buffer);
                BLE::state = BLE::State::READY;
            }
        }
    };
};

namespace dragberry
{
namespace os
{
template<typename Execution>
static void run(Execution &&execution)
{
    execution();
}
}
}

#endif
