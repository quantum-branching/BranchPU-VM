#pragma once

#include <ctype.h>

size_t exec240(const size_t cycles);

void readBin240(const char *filename);

unsigned char destination(unsigned short reg);

void unpack240();

void printState240();

void speedTest240(char flag);