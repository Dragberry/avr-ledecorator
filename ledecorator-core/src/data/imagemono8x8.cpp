#include "imagemono8x8.hpp"

const ImageMono8x8* ImageMono8x8::for_character(const char char_index)
{
	return (ImageMono8x8*) pgm_read_ptr(&(CHARS[char_index >= LAST_CHARACTER_INDEX ? 0 : char_index]));
}
