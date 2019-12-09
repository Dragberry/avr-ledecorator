#include "sensors.h"

#include <lib/avr/hardware/i2c.hpp>

#include "../../common/datatypeutils.h"

template <uint8_t pr_int, uint8_t pr_float, bool with_sign>
char* SensorTemplate<pr_int, pr_float, with_sign>::get_display_value()
{
	return display_value;
}

void TemperatureSensor::read_value()
{
	uint8_t request[1] = {0x0f};
	I2C::master_send(100, 1, request);
	uint8_t response[1];
	I2C::master_receive(100, 1, response);
	value = response[0];
}

void TemperatureSensor::convert_value()
{
	int_to_string(display_value, value, 3);
//	float_to_string(display_value, value, 2, 1, true);
}

void PressureSensor::read_value()
{
	uint8_t request[1] = {0xf0};
	I2C::master_send(100, 1, request);
	uint8_t response[1];
	I2C::master_receive(100, 1, response);
	value = response[0];
}

void PressureSensor::convert_value()
{
	int_to_string(display_value, value, 3);
}

