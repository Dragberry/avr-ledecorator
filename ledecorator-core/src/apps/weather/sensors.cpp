#include <stdlib.h>
#include "sensors.hpp"
#include "../../dragberry/os/display.hpp"

Sensor::Sensor(const Image* pictogram) :
        pictogram(pictogram)
{
    value_string.color = WHITE;
    value_string.align = DrawableString::Align::RIGHT;
}

Sensor::~Sensor()
{
}

void Sensor::set_value(int32_t new_int_value)
{
    int_value = new_int_value;
    process_value();
    value_string.set_string(string_value);
}

void Sensor::draw()
{
    display::clear_screen(BLACK);
    display::draw_image(0, 0, pictogram, BLACK);
    value_string.draw();
}

TemperatureSensor::TemperatureSensor() :
        Sensor(&IMG_TEMPERATURE)
{
}

void TemperatureSensor::process_value()
{
   char* string_pointer = string_value;
   if (int_value > 0)
   {
       string_pointer[0] = '+';
       string_pointer++;
   }
   else if (int_value < 0)
   {
       string_pointer[0] = '-';
       string_pointer++;
   }

   int abs_value = abs(int_value);
   itoa(abs_value / 100, string_pointer, 10);
   string_pointer += (abs_value > 10 ? 2 : 1);
   string_pointer[0] = '.';
   string_pointer++;
   itoa((int_value % 100) / 10, string_pointer, 10);
}

PressureSensor::PressureSensor() :
        Sensor(&IMG_PRESSURE)
{
}

void PressureSensor::process_value()
{
    itoa(int_value / 133, string_value, 10);
}