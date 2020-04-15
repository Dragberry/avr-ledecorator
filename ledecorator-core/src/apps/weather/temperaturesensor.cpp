#include <string.h>
#include "temperaturesensor.hpp"

const char TemperatureSensor::UNIT[] = "C";

TemperatureSensor::TemperatureSensor() :
        Sensor(Sensor::Code::TEMPERATURE, &IMG_TEMPERATURE, &TEMPERATURE_DB, &LAST_UPDATED_TIME)
{
    unit_string.set_string(UNIT);
}

void TemperatureSensor::load()
{
    Sensor::load();

    int16_t step = chart.get_step();
    uint8_t offset = 0;
    itoa(step / 10, step_string_value, 10);
    offset += (step > 10 ? 2 : 1);
    step_string_value[offset++] = '.';
    itoa(step % 10, step_string_value + offset, 10);
    strcat(step_string_value, UNIT);
    step_string.set_string(step_string_value);
}

void TemperatureSensor::process_value()
{
   uint8_t offset = 0;
   if (int_value > 0)
   {
       string_value[offset++] = '+';
   }
   else if (int_value < 0)
   {
       string_value[offset++] = '-';
   }

   int abs_value = abs(int_value);
   itoa(abs_value / 100, string_value + offset, 10);
   offset += (abs_value > 10 ? 2 : 1);
   string_value[offset++] = '.';
   itoa((int_value % 100) / 10, string_value + offset, 10);
}

void TemperatureSensor::update(uint32_t time)
{
    if (previous_values.add(int_value / 10, time))
    {
        Sensor::save();
    }
}

const uint32_t EEMEM TemperatureSensor::LAST_UPDATED_TIME = 0;

const RingBuffer<int16_t, 6> EEMEM TemperatureSensor::TEMPERATURE_DB = RingBuffer<int16_t, 6>();

const uint8_t PROGMEM TemperatureSensor::IMG_TEMPERATURE_DATA[128] =
{
        BLACK,  BLACK,  BLACK,  BLUE,   BLUE,   BLACK,  BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   WHITE,  WHITE,  BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   WHITE,  WHITE,  BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   WHITE,  WHITE,  BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   WHITE,  WHITE,  BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   RED,    RED,    BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   RED,    RED,    BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   RED,    RED,    BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   RED,    RED,    BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   RED,    RED,    BLUE,   BLACK,  BLACK,
        BLACK,  BLACK,  BLUE,   RED,    RED,    BLUE,   BLACK,  BLACK,
        BLACK,  BLUE,   BLUE,   RED,    RED,    BLUE,   BLUE,   BLACK,
        BLACK,  BLUE,   RED,    RED,    RED,    RED,    BLUE,   BLACK,
        BLACK,  BLUE,   RED,    RED,    RED,    RED,    BLUE,   BLACK,
        BLACK,  BLUE,   BLUE,   RED,    RED,    BLUE,   BLUE,   BLACK,
        BLACK,  BLACK,  BLUE,   BLUE,   BLUE,   BLUE,   BLACK,  BLACK,
};

const Image PROGMEM TemperatureSensor::IMG_TEMPERATURE = {8, 16, IMG_TEMPERATURE_DATA};
