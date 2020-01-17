#ifndef DRAWABLESTRING3X5_HPP_
#define DRAWABLESTRING3X5_HPP_

#include "drawablestring.hpp"

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
               );

    void draw() const;

    void set_string(const char* new_string);
};
}
}

#endif
