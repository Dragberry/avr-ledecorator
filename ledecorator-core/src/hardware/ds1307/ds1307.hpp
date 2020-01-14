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

    char seconds_d() const;

    char seconds_u() const;

    void minutes(const uint8_t minutes);

    char minutes_d() const;

    char minutes_u() const;

    void hours(const uint8_t hours);

    char hours_d() const;

    char hours_u() const;

};

}

#endif
