#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "sensorstorage.hpp"
#include "../../data/image.hpp"
#include "../../dragberry/os/drawablestring5x7.hpp"
#include "../../util/charts.hpp"

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

    const uint32_t* last_updated_time;

    SensorStorage previous_values;

    char string_value[6];

    DrawableString5x7 value_string = DrawableString5x7(8, 0, 24, 8);

    DrawableString5x7 unit_string = DrawableString5x7(8, 8, 24, 8);

    char step_string_value[5];

    DrawableString5x7 step_string = DrawableString5x7(8, 0, 24, 8);

    Histogram<int16_t, 6> chart = Histogram<int16_t, 6>(24, 8);


public:
    enum DisplayMode
    {
        VALUE,
        CHART
    } display_mode = VALUE;

public:
    Sensor(
            const Image* pictogram,
            const RingBuffer<int16_t, 6>* database,
            const uint32_t* last_updated_time
            );

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
