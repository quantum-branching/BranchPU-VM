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

#define binary binary__280
#define programCounter programCounter__280
#define registers registers__280
#define accumulator accumulator__280
#define conditionFlag conditionFlag__280
#define jmpStack jmpStack__280
#define valStack valStack__280
#define ports ports__280

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

/// The binary program loaded into the CPU, which can hold up to 4096 bytes of instructions and data
uint8_t binary[BINARY_SIZE];

/// The current program counter, which points to the current instruction in the binary
int programCounter;

/// The state of the CPU's registers, which can hold any value from 0 to 255
uint32_t registers[REGISTERS_SIZE];

/// The accumulator, which can hold any value from 0 to 255
uint32_t accumulator;

/// The condition flag, which can hold either 0 or 1
int conditionFlag;

/// The stack for storing return 11-bit addresses for jumps and calls
struct Stack jmpStack;

/// The stack for storing 8-bit values, helpful for storing temporary values
struct Stack valStack;

/// The ports for input and output
struct Port ports[8];

void init280() {
	programCounter = 0;
	accumulator = 0;
	conditionFlag = 0;
	jmpStack.index = 0;
	valStack.index = 0;

	for (int i = 0; i < REGISTERS_SIZE; i++) {
		registers[i] = 0;
	}

	for (int i = 0; i < PORTS_SIZE; i++) {
		ports[i].input = 0;
		ports[i].output = 0;
	}

	ports[1].update = GPU_p26;
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
				acc = mask(acc + regData);
				break;
			case SUB__280:
				acc = mask(acc - regData);
				break;
			case LSH__280:
				acc = mask(acc << regData);
				break;
			case RSH__280:
				acc = (acc >> regData);
				break;
			case AND__280:
				acc = (acc & regData);
				break;
			case OR__280:
				acc = (acc | regData);
				break;
			case XOR__280:
				acc = (acc ^ regData);
				break;
			case LDA__280:
				acc = regData;
				break;
			case STA__280:
				registers[data] = acc;
				break;
			case CND__280:
				if (cond) {
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
				cond = mod >> 2;
				if(((CMP_LT_FLAG & mod) && acc < registers[data]) || ((CMP_EQ_FLAG & mod) && acc == registers[data])) {
					invert(cond);
				}
				break;
			case ICP__280:
				cond = mod >> 2;
				if(((CMP_LT_FLAG & mod) && acc < data) || ((CMP_EQ_FLAG & mod) && acc == data)) {
					invert(cond);
				}
				break;
			case STK__280:
				if (mod & STK_POP_FLAG) {
					set(stack_pop(&valStack));
				} else {
					stack_push(&valStack, regData);
				}
				break;
			case RPA__280:
				acc = mask(CURRENT_PORT.output);
				break;
			case RPR__280:
				registers[DATA_OFFSET] = CURRENT_PORT.output;
				break;
			case WPA__280:
				port_handlePort(CURRENT_PORT, acc);
				break;
			case WPR__280:
				port_handlePort(CURRENT_PORT, registers[data]);
				break;
		}

		pc += INSTRUCTION_STEP;
	}
}

void printState280() {
	printf("PC: %d\n", programCounter / INSTRUCTION_STEP);
	printf("ACC: %d\n", accumulator);
	for (int i = 0; i < REGISTERS_SIZE; i++) {
		printf("\t$%d: %d\n", i, registers[i]);
	}
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

#undef binary
#undef programCounter
#undef registers
#undef accumulator
#undef conditionFlag
#undef jmpStack
#undef valStack
#undef ports