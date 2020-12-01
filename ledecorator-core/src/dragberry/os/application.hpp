#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "lib/avr/software/operators.hpp"
#include "stdint.h"

class Application
{
private:
    static const uint8_t BIT_TERMINATION = 0;

    static const uint8_t BIT_INFINITY_MODE = 1;

protected:
    volatile uint8_t state;

    volatile uint16_t time_to_live;

    volatile uint16_t time;

    void decompose_time(char* dest)
    {
        dest[0] = (char)time;
        dest[1] = (char)(time >> 8);
    }

public:
    Application() :
        state(0x00),
        time_to_live(0),
        time(0)
    {
    }

    inline
    void increment_time()
    {
        time++;
    }

    inline
    uint16_t get_time()
    {
        return time;
    }

    void ignore_ttl(bool ignore)
    {
        if (ignore)
        {
            sbi(state, BIT_INFINITY_MODE);
        }
        else
        {
            cbi(state, BIT_INFINITY_MODE);
        }
    }

    bool is_going_on()
    {
        return !check_bit(state, BIT_TERMINATION) &&
                (check_bit(state, BIT_INFINITY_MODE) || time < time_to_live);
    }

    inline
    void terminate()
    {
        sbi(state, BIT_TERMINATION);

    }
};

#endif
