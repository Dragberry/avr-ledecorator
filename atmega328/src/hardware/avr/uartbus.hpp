#ifndef UARTBUS_HPP_
#define UARTBUS_HPP_

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#if defined (__AVR_ATmega16__)
    #define UART_UDRE   UDRE
    #define UART_UCSRA  UCSRA
    #define UART_UCSRB  UCSRB
    #define UART_UCSRC  UCSRC
    #define UART_UDR    UDR
    #define UART_RXC    RXC
    #define UART_RXEN   RXEN
    #define UART_RXCIE  RXCIE
    #define UART_TXC    TXC
    #define UART_TXEN   TXEN
    #define UART_TXCIE  TXCIE
    #define UART_UBRRH  UBRRH
    #define UART_UBRRL  UBRRL
    #define UART_U2X    U2X
    #define UART_USBS   USBS
    #define UART_UCSZ0  UCSZ0
    #define UART_UCSZ1  UCSZ1
#elif defined (__AVR_ATmega328P__)
#define UART_UDRE   UDRE0
#define UART_UCSRA  UCSR0A
#define UART_UCSRB  UCSR0B
#define UART_UCSRC  UCSR0C
#define UART_UDR    UDR0
#define UART_RXC    RXC0
#define UART_RXEN   RXEN0
#define UART_RXCIE  RXCIE0
#define UART_TXC    TXC0
#define UART_TXEN   TXEN0
#define UART_TXCIE  TXCIE0
#define UART_UDRIE  UDRIE0
#define UART_UBRRH  UBRR0H
#define UART_UBRRL  UBRR0L
#define UART_U2X    U2X0
#define UART_USBS   USBS0
#define UART_UCSZ0  UCSZ00
#define UART_UCSZ1  UCSZ01
#else
    #if !defined(__COMPILING_AVR_LIBC__)
        #warning "device type not defined"
    #endif
#endif

#define UBRR_1X(BAUDRATE) ((unsigned int) ((F_CPU / (BAUDRATE * 16UL)) - 1UL))
#define UBRR_2X(BAUDRATE) ((unsigned int) ((F_CPU / (BAUDRATE * 8UL)) - 1UL))

class UartBus
{
public:
    enum BaudRate
    {
        B_4_800 = 520,
        B_9_600 = 260,
        B_230_400 = 10,
        B_250_000 = 9,
        B_500_000 = 4,
        B_1_250_000 = 1,
        B_2_500_000 = 0,
    };

    class RxHandler
    {
    public:
        virtual ~RxHandler();

        virtual void on_uart_rx_event(const uint8_t byte) = 0;
    };

    static RxHandler* rx_handler;

    static void set_rx_handler(RxHandler* rx_handler = nullptr);

    static void init(BaudRate baud);

    static void set_baud_rate(BaudRate baud);

    inline
    static void send_byte(const uint8_t byte)
    {
        UART_UDR = byte;
    }

};

#endif
