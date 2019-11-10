#ifndef COLORS_H_
#define COLORS_H_

#define Color uint8_t

#define BLACK 0b00000000
#define RED 0b00000011
#define GREEN 0b00001100
#define BLUE 0b00110000
#define YELLOW RED | GREEN
#define CYAN GREEN | BLUE
#define MAGENTA RED | BLUE
#define WHITE RED | GREEN | BLUE

#endif
