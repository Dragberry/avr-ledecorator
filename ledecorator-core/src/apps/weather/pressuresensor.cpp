#include "pressuresensor.hpp"

PressureSensor::PressureSensor() :
        Sensor(&IMG_PRESSURE, &PRESSURE_DB, &LAST_UPDATED_TIME)
{
    unit_string.set_string("MM");
}

void PressureSensor::load()
{
    Sensor::load();
    itoa(chart.get_step(), step_string_value, 10);
    step_string.set_string(step_string_value);
}

void PressureSensor::process_value()
{
    itoa(int_value / 133, string_value, 10);
}

void PressureSensor::update(uint32_t time)
{
    if (previous_values.add(int_value / 133, time))
    {
        Sensor::save();
    }
}

const uint32_t EEMEM PressureSensor::LAST_UPDATED_TIME = 0;

const RingBuffer<int16_t, 6> EEMEM PressureSensor::PRESSURE_DB = RingBuffer<int16_t, 6>();

const uint8_t PROGMEM PressureSensor::IMG_PRESSURE_DATA[128] =
{
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
        BLACK,  BLACK,  BLACK,  RED,    RED,    RED,    RED,    BLACK,
        CYAN,   BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  RED,    BLACK,
        BLACK,  CYAN,   BLACK,  CYAN,   BLACK,  BLACK,  RED,    BLACK,
        BLACK,  BLACK,  CYAN,   CYAN,   BLACK,  RED,    RED,    BLACK,
        BLACK,  CYAN,   CYAN,   CYAN,   BLACK,  BLACK,  RED,    BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  RED,    BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  RED,    RED,    RED,    BLACK,
        CYAN,   BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  RED,    BLACK,
        BLACK,  CYAN,   BLACK,  CYAN,   BLACK,  BLACK,  RED,    BLACK,
        BLACK,  BLACK,  CYAN,   CYAN,   BLACK,  RED,    RED,    BLACK,
        BLACK,  CYAN,   CYAN,   CYAN,   BLACK,  BLACK,  RED,    BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  RED,    BLACK,
        BLACK,  BLACK,  BLACK,  RED,    RED,    RED,    RED,    BLACK,
        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
};

const Image PROGMEM PressureSensor::IMG_PRESSURE = {8, 16, IMG_PRESSURE_DATA};

