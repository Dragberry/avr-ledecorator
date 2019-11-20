#include "sensorsapp.h"
#include "../../common/datatypeutils.h"

void SensorsApp::increment()
{
	if (time % refresh_period) {
		active_sensor = select_sensor();
		active_sensor->read_value();
		active_sensor->convert_value();
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
	return time < display_period;
}

void SensorsApp::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
	screen_interface.draw_image(0, 0, active_sensor->pictogram, BLACK);
	screen_interface.draw_string(
			active_sensor->get_display_value(), active_sensor->display_value_length, 8, 0, 0, 0, 24, 7, YELLOW, BLACK);
}
