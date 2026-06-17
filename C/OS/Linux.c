//// @file Linux.h
/// @brief The Linux-specific system commands for the BPU, which 
/// include clearing the screen and printing the screen

#include <time.h>
#include "OS.h"

void clear_screen() {
	system("clear");
}

void sleepms(int ms) {
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000 };
    nanosleep(&ts, NULL);
    return;
}