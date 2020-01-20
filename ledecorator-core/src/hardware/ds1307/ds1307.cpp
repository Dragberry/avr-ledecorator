#include "ds1307.hpp"
#include "lib/avr/hardware/i2c.hpp"

using namespace DS1307;

Clock::Clock(
        uint8_t address,
        Status (*device_read)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length),
        Status (*device_write)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)

) :
        address(address),
        data{0},
        device_read(device_read),
        device_write(device_write)
{
    if (device_read != nullptr && device_write != nullptr)
    {
        status = OK;
    }
}

void Clock::start()
{
    if (status == Status::OK)
    {
        const uint8_t reg_addr = 0x00;
        uint8_t data = 0;
        status = device_read(address, reg_addr, &data, 1);
        cbi(data, 7);
        status = device_write(address, reg_addr, &data, 1);
    }
}

void Clock::stop()
{
    if (status == Status::OK)
    {
        const uint8_t reg_addr = 0x00;
        uint8_t data = 0;
        status = device_read(address, reg_addr, &data, 1);
        sbi(data, 7);
        status = device_write(address, reg_addr, &data, 1);
    }
}

void Clock::refresh()
{
    if (status == Status::OK)
    {
        const uint8_t reg_addr = 0x00;
        status = device_read(address, reg_addr, data, 8);
    }
}

void Clock::update()
{
    if (status == Status::OK)
    {
        const uint8_t reg_addr = 0x00;
        status = device_write(address, reg_addr, data, 8);
    }
}


void Clock::seconds(const uint8_t seconds)
{
    data[0] = (seconds % 10) | (((seconds / 10) & 0x07) << 4);
}

uint8_t Clock::seconds_d() const
{
    return ((data[0] & 0x70) >> 4);
}

uint8_t Clock::seconds_u() const
{
    return (data[0] & 0x0F);
}

void Clock::minutes(const uint8_t minutes)
{
    data[1] = (minutes % 10) | (((minutes / 10) & 0x07) << 4);
}

uint8_t Clock::minutes_d() const
{
    return ((data[1] & 0x70) >> 4);

}

uint8_t Clock::minutes_u() const
{
    return (data[1] & 0x0F);
}

void Clock::hours(const uint8_t hours)
{
    data[2] = (hours % 10) | (((hours / 10) & 0x03) << 4);
}

uint8_t Clock::hours_d() const
{
    return ((data[2] & 0x30) >> 4);
}

uint8_t Clock::hours_u() const
{
    return (data[2] & 0x0F);
}

void Clock::days(const uint8_t days)
{
    data[4] = (days % 10) | (((days / 10) & 0x03) << 4);
}

uint8_t Clock::days_d() const
{
    return ((data[4] & 0x30) >> 4);
}

uint8_t Clock::days_u() const
{
    return (data[4] & 0x0F);
}

void Clock::months(const uint8_t months)
{
    data[5] = (months % 10) | (((months / 10) & 0x01) << 4);
}

uint8_t Clock::months_d() const
{
    return ((data[5] & 0x10) >> 4);
}

uint8_t Clock::months_u() const
{
    return (data[5] & 0x0F);
}

void Clock::years(const uint8_t years)
{
    data[6] = (years % 10) | (((years / 10) & 0x07) << 4);
}

uint8_t Clock::years_d() const
{
    return ((data[6] & 0x70) >> 4);
}

uint8_t Clock::years_u() const
{
    return (data[6] & 0x0F);
}
