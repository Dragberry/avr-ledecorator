#ifndef SORTING_HPP_
#define SORTING_HPP_

#include "stdint.h"

template <typename Type, typename Comparator>
void sort(Type* array, const int size, Comparator&& compare)
{
    for (uint8_t i = 0; i < (size - 1); i++)
    {
        for (uint8_t j = i + 1; j < size; j++)
        {
            if (compare(array[i], array[j]))
            {
                Type temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

#endif
