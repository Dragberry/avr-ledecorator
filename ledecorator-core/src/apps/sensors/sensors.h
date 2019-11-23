#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include "../../data/image.h"

class Sensor
{
public:
	const Image* pictogram;

	const uint8_t display_value_length;

	Sensor(const Image* pictogram, const uint8_t display_value_length) :
		pictogram(pictogram),
		display_value_length(display_value_length) {}

	virtual ~Sensor() {}

	virtual char* get_display_value() = 0;

	virtual void read_value() = 0;

	virtual void convert_value() = 0;
};

template <uint8_t pr_int, uint8_t pr_float, bool with_sign>
class SensorTemplate : public Sensor
{
public:
	char display_value[pr_int + pr_float + (pr_float ? 1 : 0) + (with_sign ? 1 : 0)];

	SensorTemplate(const Image* pictogram) :
		Sensor(pictogram, pr_int + pr_float + (pr_float ? 1 : 0) + (with_sign ? 1 : 0)) {}

	virtual ~SensorTemplate() {}

	char* get_display_value();

	virtual void read_value() = 0;

	void convert_value();
};


class TemperatureSensor : public SensorTemplate<2, 1, true>
{
private:
	float value = 0;

public:
	TemperatureSensor() : SensorTemplate(&IMG_TEMPERATURE) {}

	void read_value();

	void convert_value();
};

class PressureSensor : public SensorTemplate<3, 0, false>
{
private:
	uint16_t value = 0;

public:
	PressureSensor() : SensorTemplate(&IMG_PRESSURE) {}

	void read_value();

	void convert_value();
};

#endif
