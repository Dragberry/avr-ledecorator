#ifndef DUMMYAPP_HPP_
#define DUMMYAPP_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"
#include "lib/avr/hardware/i2c.hpp"
#include "../../common/datatypeutils.h"
#include "../../dragberry/os.hpp"
#include "../../dragberry/os/display.hpp"
#include "../../dragberry/os/drawablestring.hpp"
#include "../../dragberry/os/io.hpp"

#include "../../hardware/ds1307/ds1307.hpp"

using namespace dragberry::os;

class DummyApp: public Timers::T1::Handler {
private:
    DS1307::Clock clock = DS1307::Clock(
            DS1307_ADDRESS,
            device_read,
            device_write
    );

    char string[10];

    volatile uint8_t time = 0;

public:
    DummyApp() {
        I2C::init();
        I2C::set_bitrate(400);
        UART::init(UART::BaudRate::B_500_000);

        Timers::T1::start(0x4C54, Timers::Prescaller::F_1024, this);
    }

    ~DummyApp() {
        Timers::T1::stop();
    }

    void on_timer1_event() {
        time++;
        UART::send_string("Event");

        uint8_t data = 0x00;
        I2C::master_send_ni(0b11010000, 1, &data);

        I2C::master_receive_ni(0b11010000, 1, &data);
        UART::send_string("Seconds");
        UART::send_byte_as_binary(data);
    }

    void run() {
        UART::send_string("Init");

        print_all();

        uint8_t data = 0x00;
        I2C::master_send_ni(0b11010000, 1, &data);

        I2C::master_receive_ni(0b11010000, 1, &data);
        UART::send_byte_as_binary(data);

        cbi(data, 7);
        uint8_t data_w[2] = { 0x00, data };

        I2C::master_send_ni(0b11010000, 2, data_w);

        data_w[0] = 0x07;
        data_w[1] = 0b10010000;

        I2C::master_send_ni(0b11010000, 2, data_w);

        UART::send_string("Start clock");

        print_all();

        while (time < 100) {

//            dragberry::os::display::update_assured();
        }

        data = 0x00;
        I2C::master_send_ni(0b11010000, 1, &data);

        I2C::master_receive_ni(0b11010000, 1, &data);
        UART::send_string("Byte from 0x00");
        UART::send_byte_as_binary(data);

        sbi(data, 7);
        I2C::master_send_ni(0b11010000, 1, &data);
        UART::send_string("Stop clock");
    }

    static void runner() {
        DummyApp app;
        app.run();
    }

    void print_all()
    {
        uint8_t addr = 0x00;

        I2C::master_send_ni(0b11010000, 1, &addr);

        uint8_t array[8];

        I2C::master_receive_ni(0b11010000, 8, array);

        UART::send_string("All bytes:");
        for (uint8_t i = 0; i < 8; i++)
        {
            UART::send_byte_as_binary(array[i]);
        }
    }

    static DS1307::Status device_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)
    {
        if (I2C::device_read(dev_addr, reg_addr, data, length) == I2C::Status::OK)
        {
            return DS1307::Status::OK;
        }
        return DS1307::Status::DEV_NOT_FOUND;
    }

    static DS1307::Status  device_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t length)
    {
        if (I2C::device_write(dev_addr, reg_addr, data, length) == I2C::Status::OK)
        {
            return DS1307::Status::OK;
        }
        return DS1307::Status::DEV_NOT_FOUND;
    }

};

#endif
