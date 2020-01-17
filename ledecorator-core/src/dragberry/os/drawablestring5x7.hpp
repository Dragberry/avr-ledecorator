#ifndef DRAWABLESTRING5X7_HPP_
#define DRAWABLESTRING5X7_HPP_

#include "drawablestring.hpp"
#include "../../data/font5x7.hpp"

namespace dragberry
{
namespace os
{
class DrawableString5x7 : public DrawableString<5, 7>
{
public:
    DrawableString5x7(
               const uint8_t start_x,
               const uint8_t start_y,
               const int8_t width,
               const int8_t height
               ) :
                   DrawableString(start_x, start_y, width, height, Font5x7::get_char)
       {
       }

};
}
}

#endif
