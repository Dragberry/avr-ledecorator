#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#include "../common/utils.h"

class Screen {

private:
	uint8_t time;
	uint8_t sectionHeight;
	uint8_t sectionWidth;
	uint8_t sectionsHeight;
	uint8_t sectionsWidth;
	uint8_t height;
	uint8_t width;
	uint8_t currentRow;
	uint8_t** buffer;

public:
	Screen(uint8_t sectionHeight, uint8_t sectionWidth, uint8_t sectionsHeight, uint8_t sectionsWidth);
	~Screen();

	template <class L>
	void loadImage(L load) { load(buffer); }
	void drawRow(void (*sendByte)(const uint8_t byte));
	void clear();

	uint8_t getHeight();
	uint8_t getWidth();
};

#endif
