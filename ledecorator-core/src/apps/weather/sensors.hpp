#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <avr/eeprom.h>
#include <stdint.h>
#include "../../data/image.h"
#include "../../dragberry/os/drawablestring.hpp"
#include "../../util/ringbuffer.hpp"

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

    char step_string_value[5];

    DrawableString step_string = DrawableString(8, 8, 24, 8);

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

    void save();

protected:
    void process_value();
};

#endif
