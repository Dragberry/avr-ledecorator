#ifndef SENSORSAPP_H_
#define SENSORSAPP_H_

#include "../application.h"

class Sensor
{
public:
	const Image& pictogram;

	const uint8_t display_value_length;

	char* display_value;

	Sensor(const Image& pictogram, const uint8_t display_value_length) :
			pictogram(pictogram),
			display_value_length(display_value_length)
	{
		display_value = new char[display_value_length];
	}

	virtual ~Sensor()
	{
		delete [] display_value;
	}

	virtual void read_value() = 0;

	virtual void transform_value() = 0;
};

class TemperatureSensor : public Sensor
{
private:
	float value = 0;

public:
	TemperatureSensor() : Sensor(IMG_TEMPERATURE, 5) {}

	void read_value()
	{

	}

	void transform_value()
	{

	}
};

class PressureSensor : public Sensor
{
private:
	uint16_t value = 0;

public:
	PressureSensor() : Sensor(IMG_PRESSURE, 5) {}

	void read_value()
	{

	}

	void transform_value()
	{

	}
};

class SensorsApp : public Application
{
private:
	TemperatureSensor temperature_sensor = TemperatureSensor();
	PressureSensor pressure_sensor = PressureSensor();
	Sensor* sensor = &temperature_sensor;


protected:
	void increment();

public:
	bool is_running();

	void build_image(ScreenInterface& screen_interface) const;
};

#endif
