#ifndef DRAWABLESTRING5X7_HPP_
#define DRAWABLESTRING5X7_HPP_

#include "drawablestring.hpp"

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
               );

    void draw() const;

};
}
}

#endif
