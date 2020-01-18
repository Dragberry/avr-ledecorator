#ifndef CLOCKAPP_HPP_
#define CLOCKAPP_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"
#include "lib/avr/hardware/i2c.hpp"
#include "../../dragberry/os/display.hpp"
#include "../../dragberry/os/drawablestring5x7.hpp"
#include "../../dragberry/os/drawablestring3x5.hpp"
#include "../../hardware/ds1307/ds1307.hpp"

#define CLOCK_APP_TIME 1000

using namespace dragberry::os;

class ClockApp : public Timers::T1::Handler
{
private:
    volatile uint16_t time = 0;
    volatile bool update_required = false;

    DS1307::Clock clock;

    char hh_mm_string_value[6];

    DrawableString5x7 hh_mm_string;

    char ss_string_value[3];

    DrawableString3x5 ss_string;

    char date_string_value[9];

   DrawableString3x5 date_string;

public:
    ClockApp();

    static void runner();

    void run();

    void on_timer1_event();

private:
    static DS1307::Status read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length);

    static DS1307::Status write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length);
};

#endif
