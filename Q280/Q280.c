#include "Q280.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "ISA.c"
#include "Encoding.c"

#include "../Types/Stack.c"
#include "../Types/Bool.c"
#include "../IO/Port.c"
#include "../IO/GPU.h"

#define mask(value) ((value) & BYTE_MASK) /// Masks the value to be within 0 - 255, ensuring that it stays within the bounds of a byte
#define set(value) if(mod & IMMEDIATE_FLAG) { accumulator__280 = mask(value); } else { registers__280[data] = mask(value); } /// Sets either the accumulator or a register to a value, depending on the state of the immediate flag in the modifier

#define JUMP ((INSTRUCTION_STEP * ((mod << MOD_OFFSET) | data) - INSTRUCTION_STEP) & BINARY_MASK)
#define CURRENT_PORT ports__280[mod]

#define opcode (byte1 >> INSTRUCTION_OFFSET) // The opcode of the instruction (first 5 bits of byte1)
#define regData ((mod & IMMEDIATE_FLAG) ? data : registers__280[data]) // The data to be used in the instruction

#define pc programCounter__280
#define data byte2

/// The binary program loaded into the CPU, which can hold up to 4096 bytes of instructions and data
uint8_t binary[BINARY_SIZE];

/// The current program counter, which points to the current instruction in the binary
int programCounter__280;

/// The state of the CPU's registers, which can hold any value from 0 to 255
uint32_t registers__280[REGISTERS_SIZE];

/// The accumulator, which can hold any value from 0 to 255
uint32_t accumulator__280;

/// The condition flag, which can hold either 0 or 1
int conditionFlag__280;

/// The stack for storing return 11-bit addresses for jumps and calls
struct Stack jmpStack;

/// The stack for storing 8-bit values, helpful for storing temporary values
struct Stack valStack__280;

/// The ports for input and output
struct Port ports__280[8];

void init280() {
	programCounter__280 = 0;
	accumulator__280 = 0;
	conditionFlag__280 = 0;
	jmpStack.index = 0;
	valStack__280.index = 0;

	for (int i = 0; i < REGISTERS_SIZE; i++) {
		registers__280[i] = 0;
	}

	for (int i = 0; i < PORTS_SIZE; i++) {
		ports__280[i].input = 0;
		ports__280[i].output = 0;
	}

	ports__280[1].update = GPU_p26;
}

void exec280(const int cycles) {
	
	int byte1; // The first byte of the instruction
	int byte2; // The last byte of the instruction
	int mod; // The modifier of the instruction (last 3 bits of byte1)

	for (int i = 0; i < cycles; i++) {
		// Fetches the instruction and all the data that the instruction will need
		byte1 = binary[pc];
		byte2 = binary[pc + 1];
		
		mod = (byte1 & MOD_MASK);

		// Performs the instruciton
		switch (opcode) {
			case JMP__280:
				pc = JUMP;
				break;
			case ADD__280:
				accumulator__280 = mask(accumulator__280 + regData);
				break;
			case SUB__280:
				accumulator__280 = mask(accumulator__280 - regData);
				break;
			case LSH__280:
				accumulator__280 = mask(accumulator__280 << regData);
				break;
			case RSH__280:
				accumulator__280 = (accumulator__280 >> regData);
				break;
			case AND__280:
				accumulator__280 = (accumulator__280 & regData);
				break;
			case OR__280:
				accumulator__280 = (accumulator__280 | regData);
				break;
			case XOR__280:
				accumulator__280 = (accumulator__280 ^ regData);
				break;
			case LDA__280:
				accumulator__280 = regData;
				break;
			case STA__280:
				registers__280[data] = accumulator__280;
				break;
			case CND__280:
				if(conditionFlag__280) {
					pc = JUMP;
				}
				break;
			case PSH__280:
				stack_push(&jmpStack, pc);
				pc = JUMP;
				break;
			case POP__280:
				pc = stack_pop(&jmpStack) & BINARY_MASK;
				break;
			case CMP__280:
				conditionFlag__280 = mod >> 2;
				if(((CMP_LT_FLAG & mod) && accumulator__280 < registers__280[data]) || ((CMP_EQ_FLAG & mod) && accumulator__280 == registers__280[data])) {
					invert(conditionFlag__280);
				}
				break;
			case ICP__280:
				conditionFlag__280 = mod >> 2;
				if(((CMP_LT_FLAG & mod) && accumulator__280 < data) || ((CMP_EQ_FLAG & mod) && accumulator__280 == data)) {
					invert(conditionFlag__280);
				}
				break;
			case STK__280:
				if (mod & STK_POP_FLAG) {
					set(stack_pop(&valStack__280));
				} else {
					stack_push(&valStack__280, regData);
				}
				break;
			case RPA__280:
				accumulator__280 = mask(CURRENT_PORT.output);
				break;
			case RPR__280:
				registers__280[DATA_OFFSET] = CURRENT_PORT.output;
				break;
			case WPA__280:
				port_handlePort(CURRENT_PORT, accumulator__280);
				break;
			case WPR__280:
				port_handlePort(CURRENT_PORT, registers__280[data]);
				break;
		}

		pc += INSTRUCTION_STEP;
	}
}

void printState280() {
	printf("PC: \t%03d\tCAL_PTR: \t%03d\n", programCounter__280 / INSTRUCTION_STEP, jmpStack.index);
	printf("ACC: \t%03d\tVAL_PTR: \t%03d\tCND: \t  %d\n", accumulator__280, valStack__280.index,conditionFlag__280);
}

void readBin280(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (file) {
		fread(binary, 1, BINARY_SIZE, file);
		fclose(file);
	} else {
		printf("Error: Could not open file %s\n", filename);
	}
	
}

void speedTest280(char flag) {
	clock_t start = clock();
	#define CYCLES 500000000.0
	exec280((int) CYCLES);
	double time = clock() - start;
	printf("%f Hz\n", CLOCKS_PER_SEC * (CYCLES / time));

	if(flag) {
		speedTest280(flag);
	}
}