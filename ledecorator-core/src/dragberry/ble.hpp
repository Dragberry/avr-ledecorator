#ifndef BLE_HPP_
#define BLE_HPP_

#include <stdint.h>
#include "uartbus.hpp"
#include "../util/ringbuffer.hpp"

class BLE
{
private:
    static const uint8_t TIMEOUT;

    static uint8_t lifetime;

    static uint8_t counter;

public:
    static RingBuffer<uint8_t, 20> tx_buffer;

    static RingBuffer<uint8_t, 20> rx_buffer;

private:
    static class UartHandler :
            public UartBus::RxHandler,
            public UartBus::UdreHandler
    {
    public:
        ~UartHandler();

        void on_uart_rx_event(const uint8_t byte);

        void on_uart_udre_event();

    } uart_handler;

    static void timeout();

public:
    static volatile enum State
    {
        IDLE,
        PREPARING,
        READY,
        TRANSMITTING,
        RECEIVING
    } state;

    static bool is_busy();

    static bool is_connected();

    static bool start();

    static void run();

    static void stop();

    static void idle();
};

#endif
