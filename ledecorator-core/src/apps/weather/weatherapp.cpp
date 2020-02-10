#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "lib/avr/hardware/i2c.hpp"
#include "weatherapp.hpp"
#include "../../dragberry/os.hpp"

#define WEATHER_APP_TIME 36 // x0.1 second

WeatherApp::WeatherApp() :
    clock(DS1307::Clock(DS1307_ADDRESS,
            I2C::device_read<DS1307::Status, DS1307::Status::OK, DS1307::Status::DEV_NOT_FOUND>,
            I2C::device_write<DS1307::Status, DS1307::Status::OK, DS1307::Status::DEV_NOT_FOUND>
            )
    ),
    interface
    {
        BME280::InterfaceType::I2C,
        I2C::device_read<BME280::Status, BME280::Status::OK, BME280::Status::DEV_NOT_FOUND>,
        I2C::device_write<BME280::Status, BME280::Status::OK, BME280::Status::DEV_NOT_FOUND>,
        WeatherApp::delay_ms
    },
    device(BME280::Device(interface, BMP280_ADDR))
{
    I2C::init();
    I2C::set_bitrate(400);
    time = 0;
}

WeatherApp::~WeatherApp()
{
}

void WeatherApp::runner()
{
    WeatherApp app;
    app.run();
}

void WeatherApp::run()
{
    init();
    while (time < WEATHER_APP_TIME)
    {
        device.read_sensor_data(BME280_ALL, [&](BME280::Data& data) -> void
        {
            temperature_sensor.set_value(data.temperature);
            pressure_sensor.set_value(data.pressure);
        });
        active_sensor->draw();
        dragberry::os::display::update_assured();
        delay_ms(70);
        System::out::send_assured([&](RingBuffer<uint8_t, 20>& frame) -> void
        {
            char str[20] = { 0 };
            strcat(str, "T:");
            strcat(str, temperature_sensor.get_value());
            strcat(str, TemperatureSensor::UNIT);
            strcat(str, " | P:");
            strcat(str, pressure_sensor.get_value());
            strcat(str, PressureSensor::UNIT);

            uint8_t i = 0;
            while (str[i] != '\0')
            {
                frame.add(str[i++]);
            }

            frame.add('\n');
        });
    }
    uint32_t time = get_time();
    temperature_sensor.update(time);
    pressure_sensor.update(time);
    System::deregister_timer(this);
}

void WeatherApp::init()
{
    temperature_sensor.load();
    pressure_sensor.load();
    device.init([](BME280::Settings &settings) -> void
    {
        /* Recommended mode of operation: Indoor navigation */
        settings.osr_p = BME280_OVERSAMPLING_16X;
        settings.osr_t = BME280_OVERSAMPLING_2X;
        settings.filter = BME280_FILTER_COEFF_16;
        settings.standby_time = BME280_STANDBY_TIME_62_5_MS;
    });
    device.set_sensor_settings(
            BME280_OSR_PRESS_SEL |
            BME280_OSR_TEMP_SEL |
            BME280_FILTER_SEL |
            BME280_STANDBY_SEL
    );
    device.set_sensor_mode(BME280_NORMAL_MODE);
    System::register_timer(this, 100);
}

void WeatherApp::on_timer_event()
{
    time++;
    uint16_t sensor_up_time = time % 12;
    if (sensor_up_time == 0)
    {
        if (++active_sensor_index == 2)
        {
            active_sensor_index = 0;
        }
        active_sensor = sensors[active_sensor_index];
    }
    else
    {
        if (sensor_up_time < 6)
        {
            active_sensor->display_mode = Sensor::DisplayMode::VALUE;
        }
        else if (sensor_up_time < 9)
        {
            active_sensor->display_mode = Sensor::DisplayMode::CHART_Y_AXIS;
        }
        else
        {
            active_sensor->display_mode = Sensor::DisplayMode::CHART_X_AXIS;
        }
    }
}

void WeatherApp::delay_ms(uint8_t ms)
{
    while (0 < ms--)
    {
      _delay_ms(1);
    }
}

uint32_t WeatherApp::get_time()
{
    clock.refresh();
    uint32_t time = 0;
    time += (clock.seconds_d() * 10 + clock.seconds_u());
    time += ((clock.minutes_d() * 10 + clock.minutes_u()) * 60);
    time += ((clock.hours_d() * 10 + clock.hours_u()) * 360);
    time += ((clock.days_d() * 10 + clock.days_u()) * 86400);
    return time;
}
