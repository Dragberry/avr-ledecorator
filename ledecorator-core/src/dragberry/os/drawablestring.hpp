#ifndef DRAGBERRY_DRAWABLESTRING_HPP_
#define DRAGBERRY_DRAWABLESTRING_HPP_

#include <stdint.h>
#include "lib/screen/colors.h"
#include "display.hpp"

namespace dragberry
{
namespace os
{
enum Align : uint8_t
{
    LEFT, RIGHT
};

template <uint8_t fw, uint8_t fh>
class DrawableString
{
private:
    const char* string = 0;
    uint8_t string_length = 0;
    int16_t string_width = 0;
    bool is_big = false;

    const uint8_t start_x;
    const uint8_t start_y;
    const int8_t width;
    const int8_t height;

    const BitMap<fw * fh>* (*get_char)(const char c);

    int16_t offset_x = 0;

public:
    Align align = LEFT;
    Color color = WHITE;
    Color bg_color = BLACK;

    DrawableString(
            const uint8_t start_x,
            const uint8_t start_y,
            const int8_t width,
            const int8_t height,
            const BitMap<fw * fh>* (*get_char)(const char c)
            ) :

                start_x(start_x),
                start_y(start_y),
                width(width),
                height(height)
    {
        this->get_char = get_char;
    }

    void update()
    {
        if (is_big)
        {
            if (align == LEFT)
            {
                if (offset_x + string_width)
                {
                    offset_x--;
                }
                else
                {
                    offset_x = width;
                }
            }
            else
            {
                if (width - offset_x)
                {
                    offset_x++;
                }
                else
                {
                    offset_x = -string_width;
                }

            }
        }
    }

    void set_string(const char* new_string)
    {
        uint8_t length = 0;
        while (new_string[length] != '\0')
        {
            length++;
        }

        string = new_string;
        string_length = length;
        string_width = 0;
        for (uint8_t i = 0; i < length; i++)
        {
            const BitMap<fw * fh>* img = get_char(string[i]);
            string_width  += (img->get_width() + 1);
        }
        string_width--;
        if (string_width > width)
        {
            offset_x = width;
            is_big = true;
        }
        else
        {
            offset_x = 0;
            is_big = false;
        }
    }

    void draw() const
    {
        uint8_t x = start_x;
        int16_t passed_width = offset_x;
        if (!is_big && align == Align::RIGHT)
        {
            passed_width += (width - string_width);
        }
        if (passed_width > 0)
        {
            display::draw_area(x, start_y, passed_width, height, bg_color);
            if (passed_width >= width)
            {
                return;
            }
            x += passed_width;
        }
        int16_t passed_string_width = 0;
        for (uint8_t i = 0; i < string_length; i++)
        {
            const BitMap<fw * fh>* img = get_char(string[i]);
            const uint8_t img_width = img->get_width();
            passed_string_width += img_width ;
            passed_width += img_width ;
            if (passed_width < -1)
            {
                passed_width++;
                continue;
            }
            else
            {
                int16_t char_offset_x = passed_width - img_width;
                if (char_offset_x > 0)
                {
                    char_offset_x = 0;
                }

                display::draw(
                        x, start_y,
                        char_offset_x, 0,
                        img->get_width(), img->get_height(),
                        [&](const uint8_t x, const uint8_t y) -> Color
                        {
                            return img->get_bit(x, y) ? color : bg_color;
                        });

                x += (img_width  + char_offset_x);
            }
            if (passed_width >= width)
            {
                break;
            }
            passed_width++;
            if (passed_width < 0)
            {
                continue;
            }
            else if (passed_width > width)
            {
                break;
            }
            else
            {
                if (i < string_length - 1)
                {
                    display::draw_line_vertical(x++, start_y, height, bg_color);
                }
            }
        }
        if (passed_width < width)
        {
            display::draw_area(x, start_y, width - passed_width, height, bg_color);
        }
    }

};
}
}
#endif
