#include "lib/avr/software/operators.hpp"
#include "uartbus.hpp"

void UartBus::init(BaudRate baud)
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

    set_baud_rate(baud);
}

void UartBus::set_baud_rate(UartBus::BaudRate baud_rate)
{
    outb(UART_UBRRH, baud_rate >> 8);
    outb(UART_UBRRL, baud_rate);
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

ISR(USART_RX_vect)
{
    UartBus::rx_handler->on_uart_rx_event(UART_UDR);
}
