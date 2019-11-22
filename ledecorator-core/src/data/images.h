#ifndef IMAGES_H_
#define IMAGES_H_

#include <avr/pgmspace.h>
#include <stdint.h>
#include "lib/screen/colors.h"

struct Image
{
	const uint8_t width;
	const uint8_t height;
	const uint8_t* const data;
} PROGMEM;

const uint8_t IMG_TEMPERATURE_DATA[128] PROGMEM =
{
		BLACK,	BLACK,	BLACK,	BLUE,	BLUE,	BLACK,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	WHITE,	WHITE,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	WHITE,	WHITE,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	WHITE,	WHITE,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	WHITE,	WHITE,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	RED,	RED,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	RED,	RED,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	RED,	RED,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	RED,	RED,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	RED,	RED,	BLUE,	BLACK,	BLACK,
		BLACK,	BLACK,	BLUE,	RED,	RED,	BLUE,	BLACK,	BLACK,
		BLACK,	BLUE,	BLUE,	RED,	RED,	BLUE,	BLUE,	BLACK,
		BLACK,	BLUE,	RED,	RED,	RED,	RED,	BLUE,	BLACK,
		BLACK,	BLUE,	RED,	RED,	RED,	RED,	BLUE,	BLACK,
		BLACK,	BLUE,	BLUE,	RED,	RED,	BLUE,	BLUE,	BLACK,
		BLACK,	BLACK,	BLUE,	BLUE,	BLUE,	BLUE,	BLACK,	BLACK,
};

const Image IMG_TEMPERATURE PROGMEM = {8, 16, IMG_TEMPERATURE_DATA};

const uint8_t IMG_PRESSURE_DATA[128] PROGMEM =
{
		BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,
		BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,
		BLACK,	BLACK,	BLACK,	RED,	RED,	RED,	RED,	BLACK,
		CYAN,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	CYAN,	BLACK,	CYAN,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	BLACK,	CYAN,	CYAN,	BLACK,	RED,	RED,	BLACK,
		BLACK,	CYAN,	CYAN,	CYAN,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	BLACK,	BLACK,	BLACK,	RED,	RED,	RED,	BLACK,
		CYAN,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	CYAN,	BLACK,	CYAN,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	BLACK,	CYAN,	CYAN,	BLACK,	RED,	RED,	BLACK,
		BLACK,	CYAN,	CYAN,	CYAN,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	RED,	BLACK,
		BLACK,	BLACK,	BLACK,	RED,	RED,	RED,	RED,	BLACK,
		BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,	BLACK,
};

const Image IMG_PRESSURE PROGMEM = {8, 16, IMG_PRESSURE_DATA};


#endif
