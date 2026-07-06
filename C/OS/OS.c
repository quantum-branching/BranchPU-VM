//// @file OS.c
/// @brief The OS-specific system commands for the BPU, which 
/// include clearing the screen and printing the screen

#include <stdlib.h>

void clear_screen();

void sleepms(int ms);

#if defined(_WIN32) || defined(_WIN64)
  void clear_screen() {
    system("cls");
  }

  #define sleepms(x) Sleep(x)
  
#else
  void clear_screen() {
    system("clear");
  }

void sleepms(int ms) {
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000 };
    nanosleep(&ts, NULL);
    return;
}
#endif
