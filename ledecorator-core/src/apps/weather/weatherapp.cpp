#include <stdlib.h>
#include <util/delay.h>
#include "lib/avr/hardware/i2c.hpp"
#include "weatherapp.hpp"
#include "../../dragberry/os.hpp"

#define WEATHER_APP_TIME 400 // x0.1 second

WeatherApp::WeatherApp() :
        BME280::Interface(BME280::InterfaceType::I2C)
{
    I2C::init();
    I2C::set_bitrate(400);
    time = 0;
}

WeatherApp::~WeatherApp()
{
    Timers::T1::stop();
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
    }
    temperature_sensor.save();
    pressure_sensor.save();
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
    Timers::T1::start(0x7A1, Timers::Prescaller::F_1024, this);
}

void WeatherApp::on_timer1_event()
{
    time++;
    uint16_t sensor_up_time = time % 100;
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
        active_sensor->display_mode = sensor_up_time < 60 ?
                Sensor::DisplayMode::VALUE :
                Sensor::DisplayMode::CHART;
    }
}

BME280::Status WeatherApp::read(
            uint8_t dev_id,
            uint8_t reg_addr,
            uint8_t *reg_data,
            uint8_t len
            )
{
    if (I2C::device_read(dev_id, reg_addr, reg_data, len) == I2C::Status::OK)
    {
        return BME280::Status::OK;
    }
    return BME280::Status::DEV_NOT_FOUND;
}

BME280::Status WeatherApp::write(
        uint8_t dev_id,
        uint8_t reg_addr,
        uint8_t *reg_data,
        uint8_t len
        )
{
    if (I2C::device_write(dev_id, reg_addr, reg_data, len) == I2C::Status::OK)
    {
        return BME280::Status::OK;
    }
    return BME280::Status::DEV_NOT_FOUND;
}

void WeatherApp::delay_ms(uint8_t ms)
{
    while (0 < ms--)
    {
      _delay_ms(1);
    }
}
