#include "drawablestring5x7.hpp"
#include "../../data/font5x7.hpp"

using namespace dragberry::os;

DrawableString5x7::DrawableString5x7(
        const uint8_t start_x,
        const uint8_t start_y,
        const int8_t width,
        const int8_t height
        ) :
            DrawableString(start_x, start_y, width, height, Font5x7::get_char)
{
}

void DrawableString5x7::draw() const
{
    DrawableString<5, 7>::draw();
}


