#include <stdlib.h>
#include "sensors.hpp"
#include "../../dragberry/os/display.hpp"

Sensor::Sensor(const Image* pictogram, const CircularBuffer<int16_t, 6>* database) :
        pictogram(pictogram),
        database(database)
{
    value_string.color = WHITE;
    value_string.align = DrawableString::Align::RIGHT;
    eeprom_read_block(
            (void*) &previous_values,
            (const void*) database,
            sizeof(previous_values)
            );
    previous_values.reset();

}

Sensor::~Sensor()
{
    eeprom_update_block(
            (const void*) &previous_values,
            (void*) database,
            sizeof(previous_values)
            );
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

    if (previous_values.get_size() != 0)
    {
        int32_t values[previous_values.get_size()];
        previous_values.iterate([&](int16_t& element, uint8_t index) -> void
        {
            values[index] = element;
        });

        display::draw_histogram(
                8, 8,
                24, 8,
                1, 0,
                values, previous_values.get_size(),
                2, 4,
                GREEN,
                BLACK
        );
    }

    value_string.draw();
}

EEMEM CircularBuffer<int16_t, 6> TemperatureSensor::TEMPERATURE_DB = CircularBuffer<int16_t, 6>();

TemperatureSensor::TemperatureSensor() :
        Sensor(&IMG_TEMPERATURE, &TEMPERATURE_DB)
{
//    previous_values.add(2522);
//    previous_values.add(2596);
//    previous_values.add(2563);
//    previous_values.add(2501);
//    previous_values.add(2433);
//    previous_values.add(2372);
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

void TemperatureSensor::save()
{
    previous_values.add(int_value / 10);
}

EEMEM CircularBuffer<int16_t, 6> PressureSensor::PRESSURE_DB = CircularBuffer<int16_t, 6>();

PressureSensor::PressureSensor() :
        Sensor(&IMG_PRESSURE, &PRESSURE_DB)
{
//    previous_values.add(742);
//    previous_values.add(743);
//    previous_values.add(744);
//    previous_values.add(745);
//    previous_values.add(746);
//    previous_values.add(747);
}

void PressureSensor::process_value()
{
    itoa(int_value / 133, string_value, 10);
}

void PressureSensor::save()
{
    previous_values.add(int_value / 133);
}
