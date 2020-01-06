#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <stdint.h>
#include "../../data/image.h"
#include "../../dragberry/os/display.hpp"
#include "../../dragberry/os/drawablestring.hpp"

using namespace dragberry::os;

class Sensor
{
protected:
    const Image* pictogram;

    int32_t int_value = 0;

    char string_value[6];

    DrawableString value_string = DrawableString(8, 0, 24, 8);

public:
    Sensor(const Image* pictogram) :
        pictogram(pictogram)
    {
    }

    virtual ~Sensor()
    {
    }

    void set_value(int32_t int_value)
    {
        this->int_value = int_value;
    }

    void draw()
    {
        display::clear_screen(BLACK);
        display::draw_image(0, 0, pictogram, BLACK);
        value_string.draw();
    }
};

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor() :
        Sensor(&IMG_TEMPERATURE)
    {
    }

};

class PressureSensor : public Sensor
{
public:
    PressureSensor() :
        Sensor(&IMG_PRESSURE)
    {
    }
};

#endif
