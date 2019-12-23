#include "sensorsapp.hpp"
#include <lib/avr/hardware/i2c.hpp>
#include "../../common/datatypeutils.h"

SensorsApp::SensorsApp()
{
	value_string.bg_color = BLACK;
	value_string.color = RED;
	value_string.align = dragberry::os::DrawableString::Align::RIGHT;
	I2C::init();
	I2C::set_bitrate(100);
	// 1 second
	Timers::T1::start(0x4C4B, Timers::Prescaller::F_1024, this);
}

SensorsApp::~SensorsApp()
{
	Timers::T1::stop();
}

void SensorsApp::runner()
{
	SensorsApp app;
	app.run();
}

void SensorsApp::on_timer1_event()
{
	if (time++ % 5 == 0)
	{
		is_update_required = true;
	}
}

void SensorsApp::run()
{
	while (time < 25)
	{
		if (is_update_required)
		{
			active_sensor = select_sensor();
			cli();
			active_sensor->read_value();
			sei();
			active_sensor->convert_value();
			value_string.set_string(
					active_sensor->get_display_value(),
					active_sensor->display_value_length
					);
			dragberry::os::display::clear_screen(BLACK);
			dragberry::os::display::draw_image(0, 0, active_sensor->pictogram, BLACK);
			value_string.draw();
			dragberry::os::display::update_pending();
			is_update_required = false;
		}
	}
}

Sensor* SensorsApp::select_sensor()
{
	if (active_sensor_index >= SENSORS)
	{
		active_sensor_index = 0;
	}
	return sensors[active_sensor_index++];
}
