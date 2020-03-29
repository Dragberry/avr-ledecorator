#include "string.h"
#include "ble.hpp"
#include "os.hpp"

const uint8_t BLE::TIMEOUT = 50;

uint8_t BLE::lifetime = 0;

uint8_t BLE::counter = 0;

Frame BLE::rx_frame;

Frame BLE::tx_frame;

volatile bool BLE::connected = false;

volatile BLE::State BLE::state = BLE::State::IDLE;

BLE::UartHandler::~UartHandler()
{
}

void BLE::UartHandler::on_uart_rx_event(const uint8_t byte)
{
    rx_frame.add(byte);
    if (rx_frame.is_full())
    {
        System::process_event();
        stop();
    }
}

void BLE::UartHandler::on_uart_udre_event()
{
    if (!tx_frame.is_empty())
    {
        UartBus::send_byte(tx_frame.poll());
    }
    else
    {
        state = RECEIVING;
    }
}

BLE::UartHandler BLE::uart_handler = BLE::UartHandler();

bool BLE::is_connected()
{
    return check_bit(PIND, PD3);
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
        UartBus::set_rx_handler(&uart_handler);
        UartBus::set_udre_handler(&uart_handler);
        UartBus::send_byte(tx_frame.poll());
        lifetime = TIMEOUT;
        state = TRANSMITTING;
        rx_frame.clear();
    });
}

void BLE::run()
{
    switch(state)
    {
    case State::IDLE:
//        idle();
//        start();
        break;
    case State::TRANSMITTING:
    case State::RECEIVING:
        timeout();
        break;
    case State::READY:
        start();
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

void BLE::idle()
{
    state = State::PREPARING;
    char data[20] = { 0 };
    strcat(data, "[idle=");
    char value[4];
    strcat(data, itoa(counter++, value, 10));
    strcat(data, "]");

    uint8_t i = 0;
    while(i < 20)
    {
       tx_frame.add(data[i++]);
    }

    state = State::READY;
}

ISR(INT1_vect)
{
    BLE::connected = check_bit(PIND, PD3);
}

