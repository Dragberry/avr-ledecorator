#include <stdlib.h>
#include "../../dragberry/os/display.hpp"
#include "../../util/charts.hpp"
#include "sensor.hpp"

Sensor::Sensor(
        const Code code,
        const Image* pictogram,
        const RingBuffer<int16_t, 6>* database,
        const uint32_t* last_updated_time
        ) :
        code(code),
        pictogram(pictogram),
        database(database),
        last_updated_time(last_updated_time)
{
    value_string.color = WHITE;
    value_string.align = Align::RIGHT;

    step_string.color = WHITE;
    step_string.align = Align::RIGHT;

    unit_string.color = GREEN;
    unit_string.align = Align::RIGHT;

    period_string.color = WHITE;
    period_string.align = Align::RIGHT;

    period_string.set_string("1HR");
}

Sensor::~Sensor()
{
}

void Sensor::load()
{
    eeprom_read_block(
            (void*) previous_values.get_data(),
            (const void*) database,
            sizeof(RingBuffer<int16_t, 6>)
            );
    eeprom_read_block(
            (void*) previous_values.get_last_updated_time(),
            (const void*) last_updated_time,
            sizeof(uint32_t)
            );
    previous_values.reset();
    int16_t values[previous_values.get_size()];
    previous_values.get_data()->iterate([&](const int16_t& item, const uint8_t index) -> void
    {
        values[index] = item;
    });
    chart.build(values, previous_values.get_size());
}

void Sensor::save()
{
    eeprom_update_block(
           (const void*) previous_values.get_data(),
           (void*) database,
           sizeof(RingBuffer<int16_t, 6>)
           );
    eeprom_update_block(
           (const void*) previous_values.get_last_updated_time(),
           (void*) last_updated_time,
           sizeof(uint32_t)
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
    case CHART_Y_AXIS:
        step_string.draw();
        display::draw_image(8, 0, &IMG_Y_AXIS, BLACK);
        display::draw_image(8, 6, &IMG_X_AXIS, BLACK);
        display::draw_image(8, 2, &IMG_Y_AXIS_HIGHLIGHTED, BLACK);
        draw_chart();
        break;
    case CHART_X_AXIS:
        period_string.draw();
        display::draw_image(8, 0, &IMG_Y_AXIS, BLACK);
        display::draw_image(8, 6, &IMG_X_AXIS, BLACK);
        display::draw_image(11, 6, &IMG_X_AXIS_HIGHLIGHTED, BLACK);
        draw_chart();
        break;
    default:
        break;
    }
}

const char* Sensor::get_string_value()
{
    return string_value;
}

void Sensor::draw_chart()
{
    chart.draw(8, 8, 1, 0);
}

const uint8_t PROGMEM Sensor::IMG_Y_AXIS_DATA[16] =
{
        GREEN,  GREEN,
        BLACK,  GREEN,
        GREEN,  GREEN,
        BLACK,  GREEN,
        GREEN,  GREEN,
        BLACK,  GREEN,
        GREEN,  GREEN,
        BLACK,  GREEN,
};

const Image PROGMEM Sensor::IMG_Y_AXIS = {2, 8, IMG_Y_AXIS_DATA};

const uint8_t PROGMEM Sensor::IMG_Y_AXIS_HIGHLIGHTED_DATA[6] =
{
        CYAN,   CYAN,
        BLACK,  CYAN,
        CYAN,   CYAN,
};

const Image PROGMEM Sensor::IMG_Y_AXIS_HIGHLIGHTED = {2, 3, IMG_Y_AXIS_HIGHLIGHTED_DATA};

const uint8_t PROGMEM Sensor::IMG_X_AXIS_DATA[16] =
{
        GREEN,  GREEN,  GREEN,  GREEN,  GREEN,  GREEN,  GREEN,  GREEN,
        BLACK,  GREEN,  BLACK,  GREEN,  BLACK,  GREEN,  BLACK,  GREEN,
};

const Image PROGMEM Sensor::IMG_X_AXIS = {8, 2, IMG_X_AXIS_DATA};

const uint8_t PROGMEM Sensor::IMG_X_AXIS_HIGHLIGHTED_DATA[6] =
{
        CYAN,   CYAN,  CYAN,
        CYAN,   BLACK, CYAN,
};

const Image PROGMEM Sensor::IMG_X_AXIS_HIGHLIGHTED = {3, 2, IMG_X_AXIS_HIGHLIGHTED_DATA};
