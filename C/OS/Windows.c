/// @file Windows.h
/// @brief The Windows-specific system commands for the BPU, 
/// which include clearing the screen and printing the screen

#include <Windows.h>
#include "OS.h"

void clear_screen() {
	system("cls");
}

void sleepms(int ms) {
    Sleep(ms);
}