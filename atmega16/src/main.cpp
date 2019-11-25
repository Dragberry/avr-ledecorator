#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c/i2c.h"

void setup()
{
	// CTC
	TCCR1A |= (0<<WGM10);
	TCCR1A |= (0<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (0<<WGM13);
	// 000 - f
	// 100 - f/256
	// 101 - f/1024
	TCCR1B |= (1<<CS12);
	TCCR1B |= (0<<CS11);
	TCCR1B |= (1<<CS10);
	TIMSK |= (1<<OCIE1A);
	// 1s - f/1024 - 0x3D09
	OCR1A = 0x3D09;

	sei();

	I2C::init();
}

bool state = false;

uint8_t data_1[4] =
{
		'1', '2', '3', '4'
};

uint8_t data_2[4] =
{
		'5', '6', '7', '8'
};

int main()
{
	setup();
	DDRA = 0xff;
	PORTA = 0x0f;
	while(1);
}



ISR(TIMER1_COMPA_vect)
{
	PORTA = state ? 0xFF : 0x00;
	state = !state;
	I2C::master_send(100, 4, state ? data_1 : data_2);
}
