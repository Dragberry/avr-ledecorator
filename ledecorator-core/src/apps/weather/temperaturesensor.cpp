#include "temperaturesensor.hpp"

TemperatureSensor::TemperatureSensor() :
        Sensor(&IMG_TEMPERATURE, &TEMPERATURE_DB)
{
    unit_string.set_string("C");
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

void TemperatureSensor::save()
{
    previous_values.add(int_value / 10);
    Sensor::save();
}

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
