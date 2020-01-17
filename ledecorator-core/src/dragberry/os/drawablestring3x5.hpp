#ifndef DRAWABLESTRING3X5_HPP_
#define DRAWABLESTRING3X5_HPP_

#include "drawablestring.hpp"
#include "../../data/font3x5.hpp"

namespace dragberry
{
namespace os
{
class DrawableString3x5 : public DrawableString<3, 5>
{
public:
    DrawableString3x5(
               const uint8_t start_x,
               const uint8_t start_y,
               const int8_t width,
               const int8_t height
               ) :
                   DrawableString(start_x, start_y, width, height, Font3x5::get_char)

       {
       }

    void draw() const
    {
        DrawableString<3, 5>::draw();
    }
};
}
}

#endif
