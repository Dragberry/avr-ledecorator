#ifndef ARRAYLIST_HPP_
#define ARRAYLIST_HPP_

#include <stdint.h>

template <typename Type, uint8_t capacity>
class ArrayList
{
private:
    Type data[capacity] = { };

    uint8_t size = 0;

public:
    bool is_full() const
    {
        return size == capacity;
    }

    bool is_empty() const
    {
        return size == 0;
    }

    Type& get(uint8_t index)
    {
        return data[index];
    }

    bool add(Type element)
    {
        if (is_full())
        {
            return false;
        }
        data[size++] = element;
        return true;
    }

    bool remove(Type& element)
    {
        uint8_t i = 0;
        bool found = false;
        while (i < size)
        {
            if (data[i] == element)
            {
                found = true;
                break;
            }
            i++;
        }
        if (found)
        {
            for (uint8_t j = i + 1; j < size; i++, j++)
            {
                data[i] = data[j];
            }
            size--;
        }
        return true;
    }


};

#endif
