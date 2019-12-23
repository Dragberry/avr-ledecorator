#ifndef SENSORSAPP_HPP_
#define SENSORSAPP_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "../../dragberry/os.hpp"
#include "../../dragberry/os/display.hpp"
#include "../../dragberry/os/drawablestring.hpp"
#include "sensors.h"

#define SENSORS 2

class SensorsApp : public Timers::T1::Handler
{
private:
	dragberry::os::DrawableString value_string = dragberry::os::DrawableString(8, 0, 24, 8);

    TemperatureSensor temperature_sensor;
    PressureSensor pressure_sensor;

    Sensor* sensors[2] = {
        &temperature_sensor,
        &pressure_sensor
    };

    uint8_t active_sensor_index = 0;
    Sensor* active_sensor = sensors[active_sensor_index];

    volatile uint8_t time = 0;
    volatile bool is_update_required = true;


    Sensor* select_sensor();

public:
	SensorsApp();

	~SensorsApp();

	static void runner();

	void run();

	void on_timer1_event();
};

#endif
