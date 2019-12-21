#include "sensorsapp.h"

#include <lib/avr/hardware/i2c.hpp>

#include "../../common/datatypeutils.h"

SensorsApp::SensorsApp()
{
//	value_string.bg_color = BLACK;
//	value_string.color = RED;
//	value_string.align = DrawableString::Align::RIGHT;
	I2C::init();
	I2C::set_bitrate(100);
}

SensorsApp::~SensorsApp()
{

}

void SensorsApp::increment()
{
	if (display_period == 0) {
		active_sensor = select_sensor();
		active_sensor->read_value();
		active_sensor->convert_value();
//		value_string.set_string(
//				active_sensor->get_display_value(),
//				active_sensor->display_value_length);
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

bool SensorsApp::is_running()
{
	return true;
}

void SensorsApp::build_image() const
{
//	screen_interface.clear_screen(BLACK);
//	screen_interface.draw_image(0, 0, active_sensor->pictogram, BLACK);
//	value_string.draw(screen_interface);
}
