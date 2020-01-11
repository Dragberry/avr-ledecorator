#ifndef RINGBUFFER_HPP_
#define RINGBUFFER_HPP_

#include <stdint.h>
#include "lib/avr/hardware/uart.hpp"

template <typename Type, uint8_t capacity>
struct RingBuffer
{
private:
    Type buffer[capacity] = { 0 };
    uint8_t head = 0;
    uint8_t tail = 0;
    uint8_t size = 0;

    inline
    void add_inner(const Type item)
    {
        buffer[tail++] = item;
        if (tail == capacity)
        {
            tail = 0;
        }
        size++;
    }

public:
    inline
    bool is_full() const
    {
        return size == capacity;
    }

    inline
    bool is_empty() const
    {
        return size == 0;
    }

    inline
    uint8_t get_size() const
    {
        return size;
    }

    bool offer(const Type item)
    {
        if (!is_full())
        {
            add_inner(item);
            return true;
        }
        return false;
    }

    void add(const Type item)
    {
        if (is_full())
        {
            poll();
        }
        add_inner(item);
    }

    Type poll()
    {
        Type& item = buffer[head++];
        if (head == capacity)
        {
            head = 0;
        }
        size--;
        return item;
    }

    void reset()
    {
        bool is_valid = false;
        if (size <= capacity && head < capacity && tail < capacity)
        {
            if (head > tail)
            {
                is_valid = head - tail != capacity - size;
            }
            else
            {
                is_valid = tail - head != size - 1;
            }
        }
        if (!is_valid)
        {
            head = 0;
            tail = 0;
            size = 0;
        }
    }

    template <typename Action>
    void iterate_reverse(Action&& on_item) const
    {
        if (size <= capacity)
        {
            uint8_t i = 0;
            while (i < size)
            {
                const uint8_t real_index = (tail > i ? 0 : capacity) + tail - i - 1;
                on_item(buffer[real_index], i);
                i++;
            }
        }
    }

    template <typename Action>
    void iterate(Action&& on_item) const
    {
        if (size <= capacity)
        {
            uint8_t i = 0;
            while (i < size)
            {
                uint8_t real_index = head + i;
                if (real_index >= capacity)
                {
                    real_index -= capacity;
                }
                on_item(buffer[real_index], i);
                i++;
            }
        }
    }

//    void print_meta()
//    {
//        char string[4];
//        UART::send_string("Size:");
//        itoa(size, string, 10);
//        UART::send_string(string);
//
//        UART::send_string("Head:");
//        itoa(head, string, 10);
//        UART::send_string(string);
//
//
//        UART::send_string("Tail:");
//        itoa(tail, string, 10);
//        UART::send_string(string);
//    }
//
//    void print(const char* title)
//    {
//        UART::send_string(title);
//        char string[10];
//        UART::send_string("Capacity:");
//        itoa(capacity, string, 10);
//        UART::send_string(string);
//
//        print_meta();
//
//        UART::send_string("Values:");
//        iterate([&](const int16_t& element, const uint8_t index) -> void
//        {
//            itoa(element, string, 10);
//            UART::send_string(string);
//        });
//    }
};

#endif
