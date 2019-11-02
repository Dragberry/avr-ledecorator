#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "apps/application.h"
#include "apps/games/snake/snakegame.h"
#include "apps/games/life/lifegame.h"
#include "screen/atmega328/atmega328interface.h"
#include "screen/screen.h"
#include "screen/workers/byteterminalworker.h"
#include "screen/workers/defaultworker.h"
#include "screen/workers/pumpkinscreenworker.h"
#include "screen/workers/worker.h"

#define FCPU 20000000UL
#define USART_BAUDRATE  9600UL
#define UBRR ((FCPU / (USART_BAUDRATE * 16UL)) - 1)

Screen* screen;
Worker* workers[1] =
{
		new ByteTerminalWorker(),
//		new DefaultWorker(),
};
Worker* worker = workers[0];

void setup()
{
	Atmega328Interface* m328 = new Atmega328Interface(UBRR);
	screen = new Screen(m328, m328, m328);
	m328->launch();
}

int main()
{
	setup();
	while(1)
	{
		worker = workers[worker->do_work(screen)];
	}
}

ISR(TIMER0_COMPA_vect)
{
	screen->draw_row();
}

//void applyColors(Colors& colors, const uint8_t value, const uint8_t offset) {
//	uint8_t red = (value & RED);
//	if (red && LEVELS[red] >= brightnessStep) {
//		colors.red |= (1<<offset);
//	}
//	uint8_t green = (value & GREEN) >> 2;
//	if (green && LEVELS[green] >= brightnessStep) {
//		colors.green |= (1<<offset);
//	}
//	uint8_t blue = (value & BLUE) >> 4;
//	if (blue && LEVELS[blue] >= brightnessStep) {
//		colors.blue |= (1<<offset);
//	}
//}
//
//
//
//// row timer, 20ms
//ISR(TIMER0_COMPA_vect) {
//	RowColors rowColors = RowColors();
//
//
//
//
//	resetSPI();
//	for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++) {
//		// 0,1,2,3,4,5,6,7...
//		uint8_t sectionRowIdx = sectionIdx / SECTIONS_PER_ROW;
//		// 0,0,0,0,1,1,1,1...
//		Colors colors = Colors();
//		uint8_t pictureY = (H_SECTION * sectionRowIdx) + currentRow;
//		if (sectionRowIdx % 2 == 0) {
//			// not-inverted
//			// sectionRowIdx: 0
//			uint8_t sectionColumnIdx = sectionIdx % SECTIONS_PER_ROW;
//			// sectionColumnIdx: 0,1,2,3
//			// pictureY: 0,1,2,3,4,5,6,7
//			uint8_t pictureX = (W_SECTION * sectionColumnIdx);
//			for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++) {
//				uint8_t value = buffer[pictureY][pictureX + xOffset];
//				applyColors(colors, value, xOffset);
//			}
//		} else {
//			// inverted
//			// sectionRowIdx: 1
//			uint8_t sectionColumnIdx = MAX_SECTION_IDX_IN_ROW - (sectionIdx % SECTIONS_PER_ROW);
//			// sectionColumnIdx: 3,2,1,0
//			// pictureY: 8,9,10,11,12,13,14,15
//			uint8_t pictureX = (W_SECTION * sectionColumnIdx);
//			for (uint8_t xOffset = 0; xOffset < W_SECTION; xOffset++) {
//				uint8_t value = buffer[pictureY][pictureX + MAX_PIXEL_IDX_IN_SEGMENT - xOffset];
//				applyColors(colors, value, xOffset);
//			}
//		}
//		sendByteBySPI(colors.red);
//
//		sendByteBySPI(colors.green);
//
//		sendByteBySPI(colors.blue);
//
//	}
//	PORTC = ROW_L_MASK;
//	PORTB = ROW_H_MASK;
//	confirmSPI();
//	uint8_t rows = (1<<currentRow);
//	PORTC = ~(rows & ROW_L_MASK);
//	PORTB = ~((rows >> 6) & ROW_H_MASK);
//
//	if(++brightnessStep == 16) {
//		brightnessStep = 0;
//		if (++currentRow == ROWS) {
//			currentRow = 0;
//		}
//	}
//}
