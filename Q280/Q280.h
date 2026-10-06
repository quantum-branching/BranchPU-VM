#pragma once

/// @brief Initializes the CPU state
void init280();

/// @brief Executes a given number of cycles of the loaded binary
/// @param cycles The number of cycles to execute
void exec280(const int cycles);

/// @brief Prints the program counter, accumulator, and all registers in a readable format
void printState280();

/// @brief Reads the entire file and set the binary to the contents of the file, which should be a compiled BPU program
/// @param filename The file being read, which should be a compiled BPU program
void readBin280(const char *filename);

void speedTest280(char flag);