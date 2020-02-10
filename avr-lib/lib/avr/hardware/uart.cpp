#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "uart.hpp"
#include "../software/operators.hpp"

UART::RxHandler::~RxHandler() {}

void UART::set_rx_handler(RxHandler* handler)
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

UART::TxHandler::~TxHandler() {}

void UART::set_tx_handler(TxHandler* handler)
{
    tx_handler = handler;
}

UART::UdreHandler::~UdreHandler() {}

void UART::set_udre_handler(UdreHandler* handler)
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

void UART::init(UART::BaudRate baud_rate)
{
    UART_UBRRH = (uint8_t) (baud_rate >> 8);
    UART_UBRRL = (uint8_t) (baud_rate);

    sbi(UART_UCSRA, UART_U2X);

    sbi(UART_UCSRB, UART_RXEN);
    sbi(UART_UCSRB, UART_TXEN);
    // 1 stop bit
    cbi(UART_UCSRC, UART_USBS);
    // 8 bit
    sbi(UART_UCSRC, UART_UCSZ0);
    sbi(UART_UCSRC, UART_UCSZ1);
}

void UART::stop()
{
    cbi(UART_UCSRB, UART_RXEN);
    cbi(UART_UCSRB, UART_TXEN);
    set_rx_handler(nullptr);
    set_tx_handler(nullptr);
}

void UART::send_byte_as_binary(const uint8_t byte)
{
    send_byte('0');
    send_byte('b');
    uint8_t i = 0;
    while (i < 8)
    {
        send_byte((byte & (0b10000000 >> i++)) ? '1' : '0');
    }
    send_byte('\n');
}

const char HEX_VALUES[16] PROGMEM =
{
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};

void UART::send_byte_as_hex(const uint8_t byte)
{
    send_byte('0');
    send_byte('x');
    send_byte(pgm_read_byte(&HEX_VALUES[byte >> 4]));
    send_byte(pgm_read_byte(&HEX_VALUES[byte & 0x0F]));
    send_byte('\n');
}

void UART::send_string(const char* string)
{
    uint8_t index = 0;
    while (string[index] != '\0')
    {
        send_byte(string[index++]);
    }
    send_byte('\n');
}

uint8_t UART::receive_byte_ack(uint8_t ack)
{
    while (!(UART_UCSRA & (1<<UART_UDRE)));
    uint8_t byte = UART_UDR;
    UART_UDR = ack;
    return byte;
}

