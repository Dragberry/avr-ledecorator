#ifndef BLE_HPP_
#define BLE_HPP_

#include <stdint.h>
#include "uartbus.hpp"

class BLE
{
private:
    static const uint8_t TIMEOUT;

    volatile static uint8_t lifetime;

public:
    volatile static bool connected;

    static char tx_buffer[20];

    volatile static uint8_t tx_index;

    static char rx_buffer[20];

    volatile static uint8_t rx_index;

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

    static void check_connection();

    static bool start();

    static void run();

    static void stop();

    static void idle();
};

#endif
