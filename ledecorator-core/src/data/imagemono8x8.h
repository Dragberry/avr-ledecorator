#ifndef IMAGEMONO8X8_H_
#define IMAGEMONO8X8_H_

#include <avr/pgmspace.h>
#include <stdint.h>

#define OFFSET 43

struct ImageMono8x8
{
	const uint8_t width;
	const uint8_t height;
	const uint8_t data[8];

	inline const uint8_t get_width() const
	{
		 return pgm_read_byte(&width);
	}

	inline const uint8_t get_height() const
	{
		 return pgm_read_byte(&height);
	}

	inline const uint8_t get_bit(const uint8_t x, const uint8_t y) const
	{
		return (0b10000000 >> x) & pgm_read_byte(&(data[y]));
	}

	static const ImageMono8x8* for_character(const char char_index);

} PROGMEM;

const ImageMono8x8 CHAR_PLUS PROGMEM =
{
		3, 7,
		{
			0b00000000,
			0b00000000,
			0b01000000,
			0b11100000,
			0b01000000,
			0b00000000,
			0b00000000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_COMMA PROGMEM =
{
		2, 7,
		{
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b01000000,
			0b01000000,
			0b11000000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_MINUS PROGMEM =
{
		3, 7,
		{
			0b00000000,
			0b00000000,
			0b00000000,
			0b11100000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_DOT PROGMEM =
{
		1, 7,
		{
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b00000000,
			0b10000000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_SLASH PROGMEM =
{
		5, 7,
		{
			0b00000000,
			0b00001000,
			0b00010000,
			0b00100000,
			0b01000000,
			0b10000000,
			0b00000000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_0 PROGMEM =
{
		// 0
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10011000,
			0b10101000,
			0b11001000,
			0b10001000,
			0b01110000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_1 PROGMEM =
{		// 1
		5, 7,
		{
			0b00100000,
			0b01100000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b01110000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_2 PROGMEM =
{
		// 2
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b00001000,
			0b00110000,
			0b01000000,
			0b10000000,
			0b11111000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_3 PROGMEM =
{
		// 3
		5, 7,
		{
			0b11111000,
			0b00001000,
			0b00010000,
			0b00110000,
			0b00001000,
			0b10001000,
			0b01110000,
			0b00000000,
		}

};

const ImageMono8x8 CHAR_4 PROGMEM =
{
		// 4
		5, 7,
		{
			0b00010000,
			0b00110000,
			0b01010000,
			0b10010000,
			0b11111000,
			0b00010000,
			0b00010000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_5 PROGMEM =
{
		// 5
		5, 7,
		{
			0b11111000,
			0b10000000,
			0b11110000,
			0b00001000,
			0b00001000,
			0b10001000,
			0b01110000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_6 PROGMEM =
{
		// 6
		5, 7,
		{
			0b00111000,
			0b01000000,
			0b10000000,
			0b11110000,
			0b10001000,
			0b10001000,
			0b01110000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_7 PROGMEM =
{
		// 7
		5, 7,
		{
			0b11111000,
			0b00001000,
			0b00010000,
			0b00100000,
			0b01000000,
			0b01000000,
			0b01000000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_8 PROGMEM =
{
		// 8
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10001000,
			0b01110000,
			0b10001000,
			0b10001000,
			0b01110000,
			0b00000000,
		}
};

const ImageMono8x8 CHAR_9 PROGMEM =
{
		// 9
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10001000,
			0b01111000,
			0b00001000,
			0b00010000,
			0b11100000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_COLON PROGMEM =
{
		// :
		1, 7,
		{
			0b00000000,
			0b00000000,
			0b10000000,
			0b00000000,
			0b10000000,
			0b00000000,
			0b00000000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_SEMICOLON PROGMEM =
{
		// ;
		2, 7,
		{
			0b00000000,
			0b00000000,
			0b01000000,
			0b00000000,
			0b01000000,
			0b01000000,
			0b10000000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_ANGLE_BRACKET_OPEN PROGMEM =
{
		// <
		5, 7,
		{
			0b00010000,
			0b00100000,
			0b01000000,
			0b10000000,
			0b01000000,
			0b00100000,
			0b00010000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_EQ PROGMEM =
{
		// =
		5, 7,
		{
			0b00000000,
			0b00000000,
			0b11111000,
			0b00000000,
			0b11111000,
			0b00000000,
			0b00000000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_ANGLE_BRACKET_CLOSE PROGMEM =
{
		// >
		5, 7,
		{
			0b01000000,
			0b00100000,
			0b00010000,
			0b00001000,
			0b00010000,
			0b00100000,
			0b01000000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_QUESTION PROGMEM =
{
		// ?
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b00010000,
			0b00100000,
			0b00100000,
			0b00000000,
			0b00100000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_@ PROGMEM =
{
		// @
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10101000,
			0b10111000,
			0b10110000,
			0b10000000,
			0b01111000,
			0b00000000
		}
}

const ImageMono8x8 CHAR_A PROGMEM =
{
		// A
		5, 7,
		{
			0b00100000,
			0b01010000,
			0b10001000,
			0b10001000,
			0b11111000,
			0b10001000,
			0b10001000,
			0b00000000
		}
}

const ImageMono8x8 CHAR_B PROGMEM =
{
		// B
		5, 7,
		{
			0b11110000,
			0b10001000,
			0b10001000,
			0b11110000,
			0b10001000,
			0b10001000,
			0b11110000,
			0b00000000
		}
}

const ImageMono8x8 CHAR_C PROGMEM =
{
		// C
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b10001000,
			0b01110000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_D PROGMEM =
{
		// D
		5, 7,
		{
			0b11110000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b11110000,
			0b00000000
		}
};


const ImageMono8x8 CHAR_E PROGMEM =
{
		// E
		5, 7,
		{
			0b11111000,
			0b10000000,
			0b10000000,
			0b11110000,
			0b10000000,
			0b10000000,
			0b11111000,
			0b00000000
		}
};


const ImageMono8x8 CHAR_F PROGMEM =
{
		// F
		5, 7,
		{
			0b11111000,
			0b10000000,
			0b10000000,
			0b11110000,
			0b10000000,
			0b10000000,
			0b00000000,
			0b00000000
		}
};


const ImageMono8x8 CHAR_G PROGMEM =
{
		// G
		5, 7,
		{
			0b01111000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b10011000,
			0b10001000,
			0b01111000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_H PROGMEM =
{
		// H
		5, 7,
		{
			0b10001000,
			0b10001000,
			0b10001000,
			0b11111000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_I PROGMEM =
{
		// I
		5, 7,
		{
			0b00111000,
			0b00010000,
			0b00010000,
			0b00010000,
			0b00010000,
			0b00010000,
			0b00111000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_J PROGMEM =
{
		// J
		5, 7,
		{
			0b00001000,
			0b00001000,
			0b00001000,
			0b00001000,
			0b00001000,
			0b10001000,
			0b01110000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_K PROGMEM =
{
		// K
		5, 7,
		{
			0b10001000,
			0b10010000,
			0b10100000,
			0b11000000,
			0b10100000,
			0b10010000,
			0b10001000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_L PROGMEM =
{
		// L
		5, 7,
		{
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b10000000,
			0b11111000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_M PROGMEM =
{
		// M
		5, 7,
		{
			0b10001000,
			0b11011000,
			0b10101000,
			0b10101000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_N PROGMEM =
{
		// N
		5, 7,
		{
			0b10001000,
			0b10001000,
			0b11001000,
			0b10101000,
			0b10011000,
			0b10001000,
			0b10001000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_O PROGMEM =
{
		// O
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b01110000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_P PROGMEM =
{
		// P
		5, 7,
		{
			0b11110000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b11110000,
			0b10000000,
			0b10000000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_Q PROGMEM =
{
		// Q
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10101000,
			0b10010000,
			0b01101000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_R PROGMEM =
{
		// R
		5, 7,
		{
			0b11111000,
			0b10001000,
			0b10001000,
			0b11110000,
			0b10100000,
			0b10010000,
			0b10001000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_S PROGMEM =
{
		// S
		5, 7,
		{
			0b01110000,
			0b10001000,
			0b10000000,
			0b01110000,
			0b00001000,
			0b10001000,
			0b01110000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_T PROGMEM =
{
		// T
		5, 7,
		{
			0b11111000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_U PROGMEM =
{
		// U
		5, 7,
		{
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b01110000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_V PROGMEM =
{
		// V
		5, 7,
		{
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b10001000,
			0b01010000,
			0b00100000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_W PROGMEM =
{
		// W
		5, 7,
		{
			0b10001000,
			0b10001000,
			0b10001000,
			0b10101000,
			0b10101000,
			0b11011000,
			0b10001000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_X PROGMEM =
{
		// X
		5, 7,
		{
			0b10001000,
			0b10001000,
			0b01010000,
			0b00100000,
			0b01010000,
			0b10001000,
			0b10001000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_Y PROGMEM =
{
		// Y
		5, 7,
		{
			0b10001000,
			0b10001000,
			0b01010000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b00100000,
			0b00000000
		}
};

const ImageMono8x8 CHAR_Z PROGMEM =
{
		// Z
		5, 7,
		{
			0b11111000,
			0b00001000,
			0b00010000,
			0b00100000,
			0b01000000,
			0b10000000,
			0b11111000,
			0b00000000
		}
};

const ImageMono8x8* const CHARS[15] PROGMEM =
{
		&CHAR_PLUS,
		&CHAR_COMMA,
		&CHAR_MINUS,
		&CHAR_DOT,
		&CHAR_SLASH,
		&CHAR_0,
		&CHAR_1,
		&CHAR_2,
		&CHAR_3,
		&CHAR_4,
		&CHAR_5,
		&CHAR_6,
		&CHAR_7,
		&CHAR_8,
		&CHAR_9,
		&CHAR_COLON,
		&CHAR_SEMICOLON,
		&CHAR_ANGLE_BRACKET_OPEN,
		&CHAR_EQ,
		&CHAR_ANGLE_BRACKET_CLOSE,
		&CHAR_QUESTION,
		&CHAR_@,
		&CHAR_A,
		&CHAR_B,
		&CHAR_C,
		&CHAR_D,
		&CHAR_E,
		&CHAR_F,
		&CHAR_G,
		&CHAR_H,
		&CHAR_I,
		&CHAR_J,
		&CHAR_K,
		&CHAR_L,
		&CHAR_M,
		&CHAR_N,
		&CHAR_O,
		&CHAR_P,
		&CHAR_Q,
		&CHAR_R,
		&CHAR_S,
		&CHAR_T,
		&CHAR_U,
		&CHAR_V,
		&CHAR_W,
		&CHAR_X,
		&CHAR_Y,
		&CHAR_Z
};

#endif
