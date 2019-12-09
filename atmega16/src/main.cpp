#include <avr/io.h>
#include <avr/interrupt.h>

#include "lib/avr/hardware/i2c.hpp"
#include "lib/avr/hardware/i2cslavehandler.h"
#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"

class I2CHandler : public I2C::SlaveHandler
{
private:
	volatile uint8_t value = 0;
public:
	void handle_recieve(uint8_t data_length, uint8_t* data)
	{
		UART::send_string("BYTE IS RECIEVED:");
		UART::send_byte_as_binary(data[0]);
		value = data[0];
		PORTA = value;
	}

	uint8_t handle_transmit(uint8_t data_length, uint8_t* data)
	{
		UART::send_string("BYTE IS SENT:");
		UART::send_byte_as_binary(value & 0b01010101);
		data[0] = value & 0b01010101;
		return 1;
	}
} i2c_handler;

class Timer : public Timers::Handler
{
public:
	void handle()
	{

	}
} timer;

void setup()
{
	Timers::T1::start(0x3D09, Timers::Prescaller::F_1024, &timer);

	UART::init(UART::BaudRate::B_9_600);

	I2C::init();
	I2C::set_local_device_addr(100, 1);
	I2C::set_slave_handler(&i2c_handler);

	DDRA = 0xff;

	sei();
}

int main()
{
	setup();
	while(1);
}
