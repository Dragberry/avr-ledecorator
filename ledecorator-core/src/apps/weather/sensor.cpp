#include <stdlib.h>
#include "../../dragberry/os/display.hpp"
#include "../../util/charts.hpp"
#include "sensor.hpp"

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
        display::draw_image(8, 1, &IMG_DIFF, BLACK);
        chart.draw(8, 8, 1, 0);
        break;
    default:
        break;
    }
}

const uint8_t PROGMEM Sensor::IMG_DIFF_DATA[18] =
{
        BLACK,  YELLOW, BLACK,
        YELLOW, YELLOW, YELLOW,
        BLACK,  RED,    BLACK,
        BLACK,  RED,    BLACK,
        YELLOW, YELLOW, YELLOW,
        BLACK,  YELLOW, BLACK,
};

const Image PROGMEM Sensor::IMG_DIFF = {3, 6, IMG_DIFF_DATA};
