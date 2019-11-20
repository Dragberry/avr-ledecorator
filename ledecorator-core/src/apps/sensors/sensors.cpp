#include "sensors.h"
#include "../../common/datatypeutils.h"

template <uint8_t pr_int, uint8_t pr_float, bool with_sign>
char* SensorTemplate<pr_int, pr_float, with_sign>::get_display_value()
{
	return display_value;
}

void TemperatureSensor::read_value()
{
	value = 34.4;
}

void TemperatureSensor::convert_value()
{
//	display_value[0] = '+';
//	display_value[1] = '1';
//	display_value[2] = '2';
//	display_value[3] = '.';
//	display_value[4] = '5';
	float_to_string(display_value, value, 2, 1, true);
}

void PressureSensor::read_value()
{
	value = 749;
}

void PressureSensor::convert_value()
{
//	display_value[0] = '7';
//	display_value[1] = '5';
//	display_value[2] = '2';

	int_to_string(display_value, value, 3);
}

