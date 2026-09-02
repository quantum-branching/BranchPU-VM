#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "OS/OS.c"
#include "Types/Stack.c"
#include "Types/Bool.c"
#include "IO/Port.c"
#include "IO/GPU.c"
#include "Data/ISA.c"
#include "Data/Encoding.c"

#define mask(value) ((value) & BYTE_MASK) /// Masks the value to be within 0 - 255, ensuring that it stays within the bounds of a byte
#define set(value) if(mod & IMMEDIATE_FLAG) { acc = mask(value); } else { registers[data] = mask(value); } /// Sets either the accumulator or a register to a value, depending on the state of the immediate flag in the modifier

#define JUMP ((INSTRUCTION_STEP * ((mod << MOD_OFFSET) | data) - INSTRUCTION_STEP) & BINARY_MASK)
#define CURRENT_PORT ports[mod]

#define opcode (byte1 >> INSTRUCTION_OFFSET) // The opcode of the instruction (first 5 bits of byte1)
#define regData ((mod & IMMEDIATE_FLAG) ? data : registers[data]) // The data to be used in the instruction

#define acc accumulator
#define pc programCounter
#define cond conditionFlag
#define data byte2

/// @brief Initializes the CPU state
void init();

/// @brief Executes a given number of cycles of the loaded binary
/// @param cycles The number of cycles to execute
void exec(const int cycles);

/// @brief Prints the program counter, accumulator, and all registers in a readable format
void printState();

/// @brief Prints the current state of the screen, which is stored in port 1
void printScreen();

/// @brief Reads the entire file and set the binary to the contents of the file, which should be a compiled BPU program
/// @param filename The file being read, which should be a compiled BPU program
void readBinary(const char *filename);