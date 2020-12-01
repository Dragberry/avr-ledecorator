#ifndef DS1307_HPP_
#define DS1307_HPP_

#include <stdint.h>

#define DS1307_ADDRESS 0xD0

namespace DS1307
{
enum Status
{
    OK,
    NULL_PTR,
    DEV_NOT_FOUND
};

class Clock
{
public:
    Status status = NULL_PTR;

private:
    const uint8_t address;

    uint8_t data[8];

    Status (*device_read)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length);

    Status (*device_write)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length);

public:
    Clock(
            uint8_t address,
            Status (*device_read)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length),
            Status (*device_write)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)
    );

    void start();

    void stop();

    void refresh();

    void update();

    void seconds(const uint8_t seconds);

    uint8_t seconds() const;

    uint8_t seconds_d() const;

    uint8_t seconds_u() const;

    void minutes(const uint8_t minutes);

    uint8_t minutes() const;

    uint8_t minutes_d() const;

    uint8_t minutes_u() const;

    void hours(const uint8_t hours);

    uint8_t hours() const;

    uint8_t hours_d() const;

    uint8_t hours_u() const;

    void days(const uint8_t days);

    uint8_t days() const;

    uint8_t days_d() const;

    uint8_t days_u() const;

    void months(const uint8_t months);

    uint8_t months() const;

    uint8_t months_d() const;

    uint8_t months_u() const;

    void years(const uint8_t years);

    uint8_t years() const;

    uint8_t years_d() const;

    uint8_t years_u() const;

    void blink(bool enabled);
};

}

#endif
