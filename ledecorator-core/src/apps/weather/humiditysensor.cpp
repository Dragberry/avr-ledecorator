#include <stdlib.h>
#include <string.h>
#include "humiditysensor.hpp"

const char HumiditySensor::UNIT[] = "%";

HumiditySensor::HumiditySensor() :
        Sensor(Sensor::Code::HUMIDITY, &IMG_HUMIDITY, &HUMIDITY_DB, &LAST_UPDATED_TIME)
{
    unit_string.set_string(UNIT);
}

void HumiditySensor::load()
{
    Sensor::load();
    itoa(chart.get_step(), step_string_value, 10);
    strcat(step_string_value, UNIT);
    step_string.set_string(step_string_value);
}

void HumiditySensor::process_value()
{
    itoa(int_value / 1024, string_value, 10);
}

void HumiditySensor::update(uint32_t time)
{
    if (previous_values.add(int_value / 1024, time))
    {
        Sensor::save();
    }
}

const uint32_t EEMEM HumiditySensor::LAST_UPDATED_TIME = 0;

const RingBuffer<int16_t, 6> EEMEM HumiditySensor::HUMIDITY_DB = RingBuffer<int16_t, 6>();

const uint8_t PROGMEM HumiditySensor::IMG_HUMIDITY_DATA[128] =
{
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
        BLACK,  BLUE,   BLUE,   BLUE,   BLACK,  BLACK,  BLACK,  BLACK,
        BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,  BLACK,  BLACK,
        BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,  BLACK,  BLACK,
        BLUE,   BLUE,   BLUE,   BLUE,   BLUE,   BLACK,  BLACK,  BLACK,
        BLACK,  BLUE,   BLUE,   BLUE,   BLACK,  BLACK,  BLACK,  BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLUE,   BLUE,   BLUE,   BLACK,
        BLACK,  BLACK,  BLACK,  BLUE,   BLUE,   BLUE,   BLUE,   BLUE,
        BLACK,  BLACK,  BLACK,  BLUE,   BLUE,   BLUE,   BLUE,   BLUE,
        BLACK,  BLACK,  BLACK,  BLUE,   BLUE,   BLUE,   BLUE,   BLUE,
        BLACK,  BLACK,  BLACK,  BLACK,  BLUE,   BLUE,   BLUE,   BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
};

const Image PROGMEM HumiditySensor::IMG_HUMIDITY = {8, 16, IMG_HUMIDITY_DATA};
