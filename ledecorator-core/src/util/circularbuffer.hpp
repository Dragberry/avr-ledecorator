#ifndef CIRCULARBUFFER_HPP_
#define CIRCULARBUFFER_HPP_

#include <stdint.h>

template <typename Type, uint8_t capacity>
struct CircularBuffer
{
private:
    Type buffer[capacity] = { 0 };
    uint8_t cursor = 0;
    uint8_t size = 0;

    Type& get(const uint8_t index)
    {
        return buffer[capacity - 1 - index + cursor];
    }

public:
    inline uint8_t get_size()
    {
        return size;
    }

    void add(Type element)
    {
        buffer[cursor++] = element;
        if (cursor == capacity)
        {
            cursor = 0;
        }
        if (size < capacity)
        {
            size++;
        }
    }

    void reset()
    {
        if (size > capacity || cursor >= capacity)
        {
            size = 0;
            cursor = 0;
        }
    }

    template <typename Action>
    void iterate(Action&& on_element)
    {
        uint8_t i = 0;
        while (i < size)
        {
            on_element(get(size - i - 1), i);
            i++;
        }
    }
};

#endif
