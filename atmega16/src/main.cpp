#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c/i2c.h"

class I2CHandler : public I2C::SlaveHandler
{
private:
	volatile uint8_t value = 0;
public:
	void handle_recieve(uint8_t data_length, uint8_t* data)
	{
		value = data[0];
		PORTA = value;
	}

	uint8_t handle_transmit(uint8_t data_length, uint8_t* data)
	{
		data[0] = value;
		PORTA = 0xff;
		return 1;
	}
};

I2CHandler i2c_handler;

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

	DDRA = 0xff;

	I2C::init();
	I2C::set_local_device_addr(100, 1);
	I2C::set_slave_handler(&i2c_handler);

	sei();
}

int main()
{
	setup();
	while(1);
}
