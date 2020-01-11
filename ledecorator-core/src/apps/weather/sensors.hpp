#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "../../data/image.h"
#include "../../dragberry/os/drawablestring.hpp"
#include "../../util/charts.hpp"
#include "../../util/ringbuffer.hpp"
#include "../../data/bitmap.hpp"

using namespace dragberry::os;

class Sensor
{
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

class TemperatureSensor : public Sensor
{
private:
    static RingBuffer<int16_t, 6> EEMEM TEMPERATURE_DB;

public:
    TemperatureSensor();

    void load();

    void save();

protected:
    void process_value();
};

class PressureSensor : public Sensor
{
private:
    static RingBuffer<int16_t, 6> EEMEM PRESSURE_DB;

public:
    PressureSensor();

    void load();

    void save();

protected:
    void process_value();
};

const uint8_t DIFFERENCE_ICON_DATA[3] PROGMEM =
{
        /*
         * 010
         * 111
         * 010
         * 010
         * 010
         * 010
         * 111
         * 010
         */
        0b01011101,
        0b00100100,
        0b10111010
};

const BitMap DIFFERENCE_ICON PROGMEM = { 3, 8, DIFFERENCE_ICON_DATA };

#endif
