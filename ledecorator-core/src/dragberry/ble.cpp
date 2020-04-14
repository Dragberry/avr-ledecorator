#include "string.h"
#include "ble.hpp"
#include "os.hpp"
#include "lib/avr/hardware/spi.hpp"

#include "util/delay.h"

const uint8_t BLE::TIMEOUT = 5;

volatile uint8_t BLE::lifetime = 0;

char BLE::tx_buffer[20] = { 0 };

volatile uint8_t BLE::tx_index = 0;

char BLE::rx_buffer[20] = { 0 };

volatile uint8_t BLE::rx_index = 0;

volatile bool BLE::connected = false;

volatile BLE::State BLE::state = BLE::State::IDLE;

BLE::UartHandler::~UartHandler()
{
}

void BLE::UartHandler::on_uart_udre_event()
{
    if (tx_index < 20)
    {
        UartBus::send_byte(tx_buffer[tx_index++]);
        if (tx_index == 20)
        {
            rx_index = 0;
            state = RECEIVING;
            UartBus::set_rx_handler(&uart_handler);
            UartBus::set_udre_handler();
        }
    }
}

void BLE::UartHandler::on_uart_rx_event(const uint8_t byte)
{
    if (rx_index < 20)
    {
        rx_buffer[rx_index++] = byte;
        if (rx_index == 20)
        {
            System::process_event();
            stop();
        }
    }
}

BLE::UartHandler BLE::uart_handler = BLE::UartHandler();

bool BLE::is_connected()
{
    return connected;
}

void BLE::check_connection()
{
    _delay_ms(25);
    BLE::connected = check_bit(PIND, PD3);
}

bool BLE::is_busy()
{
    return state != State::IDLE;
}

void BLE::timeout()
{
    if (--lifetime == 0)
    {
        stop();
    }
}

bool BLE::start()
{
    return UartBus::acquire(PC1, UartBus::BaudRate::B_230_400, []() -> void
    {
        tx_index = 0;
        lifetime = TIMEOUT;
        state = TRANSMITTING;
        UartBus::set_udre_handler(&uart_handler);
    });
}

void BLE::run()
{
    switch(state)
    {
    case State::TRANSMITTING:
    case State::RECEIVING:
        timeout();
        break;
    default:
        break;
    }
}

void BLE::stop()
{
    if (is_busy())
    {
        UartBus::free(PC1);
        state = IDLE;
    }
}

ISR(INT1_vect)
{
    BLE::check_connection();
    if (System::current_app != nullptr)
    {
        System::current_app->ignore_ttl(BLE::connected);
    }
}

