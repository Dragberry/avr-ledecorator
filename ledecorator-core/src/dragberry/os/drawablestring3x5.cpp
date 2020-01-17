#include "drawablestring3x5.hpp"
#include "../../data/font3x5.hpp"

using namespace dragberry::os;

DrawableString3x5::DrawableString3x5(
        const uint8_t start_x,
        const uint8_t start_y,
        const int8_t width,
        const int8_t height
        ) :
            DrawableString(start_x, start_y, width, height, Font3x5::get_char)
{
}

void DrawableString3x5::draw() const
{
    DrawableString<3, 5>::draw();
}


