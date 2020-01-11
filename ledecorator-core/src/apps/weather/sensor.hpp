#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#include "../../data/image.hpp"
#include "../../dragberry/os/drawablestring.hpp"
#include "../../util/charts.hpp"
#include "../../util/ringbuffer.hpp"

using namespace dragberry::os;

class Sensor
{
private:
    static const uint8_t PROGMEM IMG_DIFF_DATA[18];

    static const Image PROGMEM IMG_DIFF;

protected:
    const Image* pictogram;

    int32_t int_value = 0;

    const RingBuffer<int16_t, 6>* database;

    RingBuffer<int16_t, 6> previous_values;

    char string_value[6];

    DrawableString value_string = DrawableString(8, 0, 24, 8);

    DrawableString unit_string = DrawableString(8, 8, 24, 8);

    char step_string_value[5];

    DrawableString step_string = DrawableString(8, 0, 24, 8);

    Histogram<int16_t, 6> chart = Histogram<int16_t, 6>(24, 8);

public:
    enum DisplayMode
    {
        VALUE,
        CHART
    } display_mode = VALUE;

public:
    Sensor(const Image* pictogram, const RingBuffer<int16_t, 6>* database);

    virtual ~Sensor();

protected:
    virtual void process_value()
    {
    }

public:
    void load();

    void save();

    void set_value(int32_t int_value);

    void draw();
};

#endif
