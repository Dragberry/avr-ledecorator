#ifndef SENSOR_HPP_
#define SENSOR_HPP_

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include "sensorstorage.hpp"
#include "../../data/image.hpp"
#include "../../dragberry/os/drawablestring3x5.hpp"
#include "../../dragberry/os/drawablestring5x7.hpp"
#include "../../util/charts.hpp"

using namespace dragberry::os;

class Sensor
{
public:
    const enum Code {
        TEMPERATURE = 'T',
        PRESSURE    = 'P',
        IDLE        = 'I',
    } code;

private:
    static const uint8_t PROGMEM IMG_Y_AXIS_DATA[16];

    static const Image PROGMEM IMG_Y_AXIS;

    static const uint8_t PROGMEM IMG_Y_AXIS_HIGHLIGHTED_DATA[6];

    static const Image PROGMEM IMG_Y_AXIS_HIGHLIGHTED;

    static const uint8_t PROGMEM IMG_X_AXIS_DATA[16];

    static const Image PROGMEM IMG_X_AXIS;

    static const uint8_t PROGMEM IMG_X_AXIS_HIGHLIGHTED_DATA[6];

    static const Image PROGMEM IMG_X_AXIS_HIGHLIGHTED;

protected:
    const Image* pictogram;

    int32_t int_value = 0;

    const RingBuffer<int16_t, 6>* database;

    const uint32_t* last_updated_time;

    SensorStorage previous_values;

    char string_value[6];

    DrawableString5x7 value_string = DrawableString5x7(8, 0, 24, 7);

    DrawableString5x7 unit_string = DrawableString5x7(8, 8, 24, 7);

    char step_string_value[7];

    DrawableString3x5 step_string = DrawableString3x5(16, 1, 16, 5);

    DrawableString3x5 period_string = DrawableString3x5(16, 1, 16, 5);

    Histogram<int16_t, 6> chart = Histogram<int16_t, 6>(24, 8);

public:
    enum DisplayMode
    {
        VALUE,
        CHART_X_AXIS,
        CHART_Y_AXIS
    } display_mode = VALUE;

public:
    Sensor(
            const Code code,
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

    inline
    int32_t get_value() const
    {
        return int_value;
    }

    void set_value(int32_t int_value);

    void draw();

    const char* get_string_value();
private:
    void draw_chart();
};

#endif
