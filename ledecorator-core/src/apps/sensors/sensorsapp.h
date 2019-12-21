#ifndef SENSORSAPP_H_
#define SENSORSAPP_H_

#include "sensors.h"


#define SENSORS 2

class SensorsApp
{
private:
    const uint8_t refresh_period = 64;
    const uint16_t display_period = 128;

//    DrawableString value_string = DrawableString(8, 0, 24, 8);

    TemperatureSensor temperature_sensor;
    PressureSensor pressure_sensor;

    Sensor* sensors[2] = {
        &temperature_sensor,
        &pressure_sensor
    };

    uint8_t active_sensor_index = 0;
    Sensor* active_sensor = sensors[active_sensor_index];

    Sensor* select_sensor();

protected:
	void increment();

public:
	SensorsApp();

	~SensorsApp();

	bool is_running();

	void build_image() const;
};

#endif
