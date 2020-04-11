#ifndef DATAFRAME_HPP_
#define DATAFRAME_HPP_

#include "stdint.h"

class DataFrame
{
private:
    const int8_t size = 20;

    uint8_t data[size] = { };

    int8_t tail = -1;

    int8_t head = -1;

public:
    void reset()
    {
        tail = -1;
        head = -1;
    }

    bool is_empty()
    {
        return tail == head;
    }

    bool is_full()
    {
        return (head + 1) == size;
    }

    void push(uint8_t byte)
    {
        data[++head];
    }

    uint8_t poll()
    {
        return data[++tail];
    }

};

#endif
