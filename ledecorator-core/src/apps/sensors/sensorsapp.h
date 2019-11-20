#ifndef SENSORSAPP_H_
#define SENSORSAPP_H_

#include "../application.h"
#include "sensors.h"

#define SENSORS 2

class SensorsApp : public Application
{
private:
    uint8_t refresh_period = 16;
    uint8_t display_period = 80;

    TemperatureSensor temperature_sensor;
    PressureSensor pressure_sensor;

    Sensor* sensors[2] = {
        &temperature_sensor,
        &pressure_sensor
    };

    uint8_t active_sensor_index;
    Sensor* active_sensor = sensors[active_sensor_index];

    Sensor* select_sensor();

protected:
	void increment();

public:
	bool is_running();

	void build_image(ScreenInterface& screen_interface) const;
};

#endif
