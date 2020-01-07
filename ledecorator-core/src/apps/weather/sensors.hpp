#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <stdint.h>
#include "../../data/image.h"
#include "../../dragberry/os/drawablestring.hpp"

using namespace dragberry::os;

class Sensor
{
protected:
    const Image* pictogram;

    int32_t int_value = 0;
    int32_t previous_values[6];

    char string_value[6];

    DrawableString value_string = DrawableString(8, 0, 24, 8);

public:
    Sensor(const Image* pictogram);

    virtual ~Sensor();

protected:
    virtual void process_value()
    {
    }

public:
    void set_value(int32_t int_value);

    void draw();
};

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor();

protected:
    void process_value();
};

class PressureSensor : public Sensor
{
public:
    PressureSensor();

protected:
    void process_value();
};

#endif
