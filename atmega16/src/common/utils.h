#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>

void * operator new(size_t size);

void operator delete(void * ptr);

void * operator new[](size_t size);

void operator delete[](void * ptr);

#endif
