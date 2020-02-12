#include "ble.hpp"
#include "os.hpp"

const uint8_t BLE::TIMEOUT = 50;

uint8_t BLE::lifetime = 0;

uint8_t BLE::counter = 0;

Frame BLE::rx_frame;

Frame BLE::tx_frame;

volatile BLE::State BLE::state = BLE::State::IDLE;

BLE::UartHandler::~UartHandler()
{
}

void BLE::UartHandler::on_uart_rx_event(const uint8_t byte)
{
    rx_frame.add(byte);
    if (rx_frame.is_full())
    {
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
    return true;
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

bool BLE::start(void (*callback)())
{
    return true;
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
        idle();
        start();
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
    char counter_string[4];
    itoa(counter++, counter_string, 10);


    tx_frame.add('i');
    tx_frame.add('d');
    tx_frame.add('l');
    tx_frame.add('e');

    uint8_t i = 0;
    while(counter_string[i] != '\0')
    {
       tx_frame.add(counter_string[i++]);
    }

    tx_frame.add('\n');
    state = State::READY;
}
