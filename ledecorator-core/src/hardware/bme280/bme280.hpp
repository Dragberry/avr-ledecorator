#ifndef BME280_HPP_
#define BME280_HPP_

#include <stdint.h>
#include "bme280defs.hpp"

namespace BME280
{

/*!
 * @brief Device status Enums
 */
enum Status
{
   OK,
   NULL_PTR,
   DEV_NOT_FOUND,
   INVALID_LEN,
   COMM_FAIL,
   SLEEP_MODE_FAIL,
   NVM_COPY_FAILED,
   INVALID_OSR_MACRO
};

/*!
 * @brief Calibration data
 */
struct CalibrationData
{
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;
  uint16_t dig_P1;
  int16_t dig_P2;
  int16_t dig_P3;
  int16_t dig_P4;
  int16_t dig_P5;
  int16_t dig_P6;
  int16_t dig_P7;
  int16_t dig_P8;
  int16_t dig_P9;
  uint8_t dig_H1;
  int16_t dig_H2;
  uint8_t dig_H3;
  int16_t dig_H4;
  int16_t dig_H5;
  int8_t dig_H6;
  int32_t t_fine;
};

/*!
 * @brief bme280 sensor settings structure which comprises of mode,
 * oversampling and filter settings.
 */
struct Settings
{
    /*! pressure oversampling */
    uint8_t osr_p;

    /*! temperature oversampling */
    uint8_t osr_t;

    /*! humidity oversampling */
    uint8_t osr_h;

    /*! filter coefficient */
    uint8_t filter;

    /*! standby time */
    uint8_t standby_time;
};

struct Data
{
    /*! Compensated pressure */
    uint32_t pressure;

    /*! Compensated temperature */
    int32_t temperature;

    /*! Compensated humidity */
    uint32_t humidity;
};

/*!
 * @brief Interface selection Enums
 */
enum InterfaceType
{
   SPI, I2C
};

class Interface
{
public:
    const InterfaceType value;

    Interface(InterfaceType value) : value(value) { }

    virtual ~Interface() { }

    virtual Status read(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint8_t len) = 0;

    virtual Status write(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint8_t len) = 0;

    virtual void delay_ms(uint8_t ms);
};

class Device
{
private:
    Interface* interface;

    uint8_t chip_id;

    uint8_t dev_id;

    CalibrationData calib_data;

    Settings settings;

public:
    Status status;

    Device(Interface* interface, uint8_t dev_id);

    void init(void (*setup_settings)(Settings& settings));

    void get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len);

    void set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len);

    void get_sensor_settings();

    void set_sensor_settings(uint8_t desired_settings);

    uint8_t get_sensor_mode();

    void set_sensor_mode(uint8_t sensor_mode);

    void soft_reset();

    void read_sensor_data(const uint8_t sensor_data, Data& data);

    template <typename DataProcessor>
    void read_sensor_data(const uint8_t sensor_data, DataProcessor&& process_data)
    {
        Data data = { 0 };
        read_sensor_data(sensor_data, data);
        process_data(data);
    }

    void parse_sensor_data(const uint8_t *reg_data, Data& uncomp_data);

    void compensate_sensor_data(uint8_t sensor_comp, Data& uncomp_data, Data& comp_data);

    int32_t compensate_temperature(const Data& uncomp_data);

    uint32_t compensate_pressure(const Data& uncomp_data);

private:
    void sleep();

    void reload_device_settings();

    void set_osr_settings(uint8_t desired_settings);

    void set_osr_humidity_settings();

    void set_osr_press_temp_settings(uint8_t desired_settings);

    void fill_osr_press_settings(uint8_t *reg_data);

    void fill_osr_temp_settings(uint8_t *reg_data);

    void set_filter_standby_settings(uint8_t desired_settings);

    void fill_filter_settings(uint8_t *reg_data);

    void fill_standby_settings(uint8_t *reg_data);

    void parse_device_settings(const uint8_t *reg_data);

    void get_calib_data();

    void parse_temp_press_calib_data(const uint8_t *reg_data);

    void parse_humidity_calib_data(const uint8_t *reg_data);

    void interleave_reg_addr(const uint8_t *reg_addr, uint8_t *temp_buff, const uint8_t *reg_data, uint8_t len);

    void null_ptr_check();

    bool are_settings_changed(uint8_t sub_settings, uint8_t desired_settings);

    void write_power_mode(uint8_t sensor_mode);
};

}

#endif
