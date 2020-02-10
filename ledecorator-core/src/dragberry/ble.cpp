#include "ble.hpp"
#include "os.hpp"

const uint8_t BLE::TIMEOUT = 50;

uint8_t BLE::lifetime = 0;

uint8_t BLE::counter = 0;

RingBuffer<uint8_t, 20> BLE::rx_buffer;

RingBuffer<uint8_t, 20> BLE::tx_buffer;

volatile BLE::State BLE::state = BLE::State::IDLE;

BLE::UartHandler::~UartHandler()
{
}

void BLE::UartHandler::on_uart_rx_event(const uint8_t byte)
{
    rx_buffer.offer(byte);
    if (rx_buffer.is_full())
    {
        stop();
    }
}

void BLE::UartHandler::on_uart_udre_event()
{
    if (!tx_buffer.is_empty())
    {
        UartBus::send_byte(tx_buffer.poll());
    }
    else
    {
        state = RECEIVING;
        stop();
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

bool BLE::start()
{
    return UartBus::acquire(PC1, UartBus::BaudRate::B_230_400, []() -> void
    {
        UartBus::set_rx_handler(&uart_handler);
        UartBus::set_udre_handler(&uart_handler);
        UartBus::send_byte(tx_buffer.poll());
        lifetime = TIMEOUT;
        state = TRANSMITTING;
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
        tx_buffer.clear();
        rx_buffer.clear();
        UartBus::free(PC1);
        state = IDLE;
    }
}

void BLE::idle()
{
    state = State::PREPARING;
    char counter_string[4];
    itoa(counter++, counter_string, 10);


    tx_buffer.add('i');
    tx_buffer.add('d');
    tx_buffer.add('l');
    tx_buffer.add('e');

    uint8_t i = 0;
    while(counter_string[i] != '\0')
    {
       tx_buffer.add(counter_string[i++]);
    }

    tx_buffer.add('\n');
    state = State::READY;
}
