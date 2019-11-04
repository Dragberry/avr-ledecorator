#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#define FCPU 20000000UL
#define USART_BAUDRATE  9600UL
#define UBRR ((FCPU / (USART_BAUDRATE * 16UL)) - 1)

void send_byte(const uint8_t data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

int main()
{
	UBRR0H = (uint8_t) (UBRR >> 8);
	UBRR0L = (uint8_t) UBRR;
	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<TXEN0);
	// 1 stop bit
	UCSR0C |= (0<<USBS0);
	// 8 bit
	UCSR0C |= (1<<UCSZ00);
	UCSR0C |= (1<<UCSZ01);


	DDRC |= (1<<PC0) | (1<<PC1);
	PORTC |= (1<<PC0) | (1<<PC1);

	uint8_t temp = 0;
	uint8_t color_1 = 0b00110000;
	uint8_t color_2 = 0b00001111;
	while(1)
	{
//		for (uint8_t y = 0; y < 16; y++)
//		{
//			for (uint8_t x = 0; x < 32; x++)
//			{
//				send_byte(0b00001111);
//			}
//		}
		_delay_ms(500);
		send_byte(0b01000000);
		for (uint8_t y = 0; y < 8; y++)
		{
			for (uint8_t x = 0; x < 32; x++)
			{
				send_byte(color_1);
			}
		}
		for (uint8_t y = 8; y < 16; y++)
		{
			for (uint8_t x = 0; x < 32; x++)
			{
				send_byte(color_2);
			}
		}
		if (temp % 2)
		{
			color_1 = 0b00110000;
			color_2 = 0b00000011;
		}
		else
		{
			color_1 = 0b00001100;
			color_2 = 0b00110011;
		}
	}

	return 0;
}

//ISR(TIMER0_COMPA_vect)
//{
//	screen.draw_row();
//}
