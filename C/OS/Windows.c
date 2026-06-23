/// @file Windows.h
/// @brief The Windows-specific system commands for the BPU, 
/// which include clearing the screen and printing the screen

#include <Windows.h>

void clear_screen() {
	system("cls");
}

#define sleepms(x) Sleep(x)