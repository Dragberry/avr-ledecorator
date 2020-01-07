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
    display::draw_histogram(
            8, 8,
            24, 8,
            1, 0,
            previous_values, 6,
            2, 4,
            GREEN,
            BLACK
    );
    value_string.draw();
}

TemperatureSensor::TemperatureSensor() :
        Sensor(&IMG_TEMPERATURE)
{
    previous_values[0] = 2522;
    previous_values[1] = 2596;
    previous_values[2] = 2563;
    previous_values[3] = 2501;
    previous_values[4] = 2433;
    previous_values[5] = 2372;
}

void TemperatureSensor::process_value()
{
   uint8_t offset = 0;
   if (int_value > 0)
   {
       string_value[offset++] = '+';
   }
   else if (int_value < 0)
   {
       string_value[offset++] = '-';
   }

   int abs_value = abs(int_value);
   itoa(abs_value / 100, string_value + offset, 10);
   offset += (abs_value > 10 ? 2 : 1);
   string_value[offset++] = '.';
   itoa((int_value % 100) / 10, string_value + offset, 10);
}

PressureSensor::PressureSensor() :
        Sensor(&IMG_PRESSURE)
{
    previous_values[0] = 742;
    previous_values[1] = 742;
    previous_values[2] = 742;
    previous_values[3] = 743;
    previous_values[4] = 743;
    previous_values[5] = 742;
}

void PressureSensor::process_value()
{
    itoa(int_value / 133, string_value, 10);
}
