#include <stdlib.h>
#include "sensors.hpp"
#include "../../dragberry/os/display.hpp"
#include "../../util/charts.hpp"

Sensor::Sensor(const Image* pictogram, const RingBuffer<int16_t, 6>* database) :
        pictogram(pictogram),
        database(database)
{
    value_string.color = WHITE;
    value_string.align = DrawableString::Align::RIGHT;

    step_string.color = WHITE;
    step_string.align = DrawableString::Align::RIGHT;

    unit_string.color = GREEN;
    unit_string.align = DrawableString::Align::RIGHT;
}

Sensor::~Sensor()
{
}

void Sensor::load()
{
    eeprom_read_block(
            (void*) &previous_values,
            (const void*) database,
            sizeof(previous_values)
            );
    previous_values.reset();
    int16_t values[previous_values.get_size()];
    previous_values.iterate([&](const int16_t& item, const uint8_t index) -> void
    {
        values[index] = item;
    });
    chart.build(values, previous_values.get_size());
}

void Sensor::save()
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
    switch (display_mode)
    {
    case VALUE:
        value_string.draw();
        unit_string.draw();
        break;
    case CHART:
        step_string.draw();
        display::draw_image(
                8, 0,
                0, 0,
                &DIFFERENCE_ICON,
                GREEN, BLACK
                );
        chart.draw(8, 8, 1, 0);
        break;
    default:
        break;
    }
}

RingBuffer<int16_t, 6> EEMEM TemperatureSensor::TEMPERATURE_DB = RingBuffer<int16_t, 6>();

TemperatureSensor::TemperatureSensor() :
        Sensor(&IMG_TEMPERATURE, &TEMPERATURE_DB)
{
    unit_string.set_string("C");
}

void TemperatureSensor::load()
{
    Sensor::load();

    int16_t step = chart.get_step();
    uint8_t offset = 0;
    itoa(step / 10, step_string_value, 10);
    offset += (step > 10 ? 2 : 1);
    step_string_value[offset++] = '.';
    itoa(step % 10, step_string_value + offset, 10);

    step_string.set_string(step_string_value);
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
    Sensor::save();
}

RingBuffer<int16_t, 6> EEMEM PressureSensor::PRESSURE_DB = RingBuffer<int16_t, 6>();

PressureSensor::PressureSensor() :
        Sensor(&IMG_PRESSURE, &PRESSURE_DB)
{
    unit_string.set_string("MM");
}

void PressureSensor::load()
{
    Sensor::load();
    itoa(chart.get_step(), step_string_value, 10);
    step_string.set_string(step_string_value);
}

void PressureSensor::process_value()
{
    itoa(int_value / 133, string_value, 10);
}

void PressureSensor::save()
{
    previous_values.add(int_value / 133);
    Sensor::save();
}
