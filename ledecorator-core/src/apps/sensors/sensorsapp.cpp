#include "sensorsapp.h"
#include "../../common/datatypeutils.h"

void SensorsApp::increment()
{
	if (time % 2)
	{
		sensor = &temperature_sensor;
	}
	else
	{
		sensor = &pressure_sensor;
	}
	float_to_string(sensor->display_value, 35.2, 2, 1, true);
}

bool SensorsApp::is_running()
{
	return true;
}

void SensorsApp::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
	screen_interface.draw_image(0, 0, sensor->pictogram, BLACK);
	screen_interface.draw_string(
			sensor->display_value, sensor->display_value_length, 8, 0, 0, 0, 24, 7, YELLOW, BLACK);
}
