#include "lib/avr/software/operators.hpp"
#include "uartbus.hpp"

volatile bool UartBus::is_busy = false;

void UartBus::init()
{
    sbi(UART_UCSRA, UART_U2X);

    sbi(UART_UCSRB, UART_RXEN);
    sbi(UART_UCSRB, UART_TXEN);
    // 1 stop bit
    cbi(UART_UCSRC, UART_USBS);
    // 8 bit
    sbi(UART_UCSRC, UART_UCSZ0);
    sbi(UART_UCSRC, UART_UCSZ1);
    // control
    sbi(DDRC, PC0);
    sbi(DDRC, PC1);
}

void UartBus::set_baud_rate(UartBus::BaudRate baud_rate)
{
    outb(UART_UBRRH, baud_rate >> 8);
    outb(UART_UBRRL, baud_rate);
}

bool UartBus::acquire(uint8_t device, UartBus::BaudRate baud_rate, void (*on_success)())
{
//    cli();
    if (is_busy)
    {
        return false;
    }
    is_busy = true;
    set_baud_rate(baud_rate);
    sbi(PORTC, device);
    on_success();
//    sei();
    return true;
}

void UartBus::free(uint8_t device)
{
//    cli();
    while (!check_bit(UART_UCSRA, UART_TXC));
    set_rx_handler();
    set_tx_handler();
    set_udre_handler();
    cbi(PORTC, device);
    is_busy = false;
//    sei();
}

UartBus::RxHandler::~RxHandler()
{
}

UartBus::RxHandler* UartBus::rx_handler = nullptr;

void UartBus::set_rx_handler(UartBus::RxHandler* handler)
{
    rx_handler = handler;
    if (handler != nullptr)
    {
        sbi(UART_UCSRB, UART_RXCIE);
    }
    else
    {
        cbi(UART_UCSRB, UART_RXCIE);
    }
}

UartBus::TxHandler::~TxHandler()
{
}

UartBus::TxHandler* UartBus::tx_handler = nullptr;

void UartBus::set_tx_handler(UartBus::TxHandler* handler)
{
    tx_handler = handler;
    if (handler != nullptr)
    {
        sbi(UART_UCSRB, UART_TXCIE);
    }
    else
    {
        cbi(UART_UCSRB, UART_TXCIE);
    }
}

UartBus::UdreHandler::~UdreHandler()
{
}


UartBus::UdreHandler* UartBus::udre_handler = nullptr;

void UartBus::set_udre_handler(UartBus::UdreHandler* handler)
{
    udre_handler = handler;
    if (handler != nullptr)
    {
        sbi(UART_UCSRB, UART_UDRIE);
    }
    else
    {
        cbi(UART_UCSRB, UART_UDRIE);
    }
}

ISR(USART_RX_vect)
{
    UartBus::rx_handler->on_uart_rx_event(UART_UDR);
}

ISR(USART_TX_vect)
{
    UartBus::tx_handler->on_uart_tx_event();
}

ISR(USART_UDRE_vect)
{
    UartBus::udre_handler->on_uart_udre_event();
}
