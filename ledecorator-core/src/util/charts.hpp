#ifndef CHARTS_HPP_
#define CHARTS_HPP_

#include <stdint.h>
#include "lib/screen/colors.h"
#include "../dragberry/os/display.hpp"

template <typename T, uint8_t capacity>
class Histogram
{
public:
    uint8_t width;
    uint8_t height;

    uint8_t line_width = 2;
    uint8_t section_width = 4;

    Color line_color = GREEN;
    Color first_line_color = CYAN;
    Color bg_color = BLACK;

private:
    T step;
    uint8_t data_set_size = 0;
    uint8_t normalized_data_set[capacity];
    uint8_t chart_width = 0;

public:
    Histogram(const uint8_t width, const uint8_t height)
    {
        this->width = width;
        this->height = height;
    }

    inline
    T get_step()
    {
        return step;
    }

    void build(const T* data_set, const uint8_t data_set_size)
    {
        this->data_set_size = data_set_size;
        chart_width = section_width * data_set_size;

        T min = data_set[0];
        T max = data_set[0];
        uint8_t index = 1;
        while (index < data_set_size)
        {
            T value = data_set[index++];
            if (value > max)
            {
                max = value;
            }
            if (value < min)
            {
                min = value;
            }
        }

        uint8_t base = 1;
        T amplitude = max - min;
        step = amplitude / height;
        if (step == 0)
        {
            step = 1;
            base = (height - amplitude) / 2;
        }

        index = 0;
        while (index < capacity)
        {
            normalized_data_set[index] =
                    index < data_set_size ? base + ((data_set[index] - min) / step) : 0;
            index++;
        }

        chart_width = section_width * data_set_size;
    }

    void draw(
            uint8_t start_x,
            uint8_t start_y,
            const int8_t offset_x,
            const int8_t offset_y)
    {
        dragberry::os::display::draw(
                start_x, start_y,
                offset_x, offset_y,
                width, height,
                [&](const uint8_t x, const uint8_t y) -> Color
                {
                    uint8_t line_index = x / section_width;
                    if (x % 4 < line_width && x < chart_width &&
                        normalized_data_set[line_index] >= height - y)
                    {
                        return line_index == data_set_size - 1 &&
                                normalized_data_set[line_index] == height - y ? first_line_color : line_color;

                    }
                    return bg_color;
                }
            );
    }
};

#endif
