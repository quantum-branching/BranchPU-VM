#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "OS/OS.h" //Includes platform-specific system commands

#include "Types/Stack.c"
#include "Types/Bool.c"
#include "IO/Port.c"
#include "IO/GPU.c"

#include "Data/ISA.c"
#include "Data/Encoding.c"

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

/// @brief Initializes the CPU state
void init() {
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

/// @brief Executes a given number of cycles of the loaded binary
/// @param cycles The number of cycles to execute
void exec(const int cycles) {
	#define JUMP (INSTRUCTION_STEP * (((mod) << MOD_OFFSET) | (data)) - INSTRUCTION_STEP) & BINARY_MASK
	#define CURRENT_PORT ports[mod & MOD_MASK]

	/// Masks the value to be within 0 - 255, ensuring that it stays within the bounds of a byte
	#define mask(value) ((value) & BYTE_MASK)

	/// Sets either the accumulator or a register to a value, depending on the state of the immediate flag in the modifier
	#define set(value) if(mod & IMMEDIATE_FLAG) { accumulator = mask(value); } else { registers[data] = mask(value); }

	int mod;
	int opcode;
	int data;
	int regData;

	for (int i = 0; i < cycles; i++) {
		//Fetches the instruction and all the data that the instruction will need
		opcode = (binary[programCounter] >> INSTRUCTION_OFFSET) & INSTRUCTION_MASK;
		data = binary[programCounter + DATA_OFFSET] & BYTE_MASK;
		mod = binary[programCounter] & MOD_MASK;
		if (!(mod & IMMEDIATE_FLAG) || opcode == CMP || opcode == WPR) {
			regData = registers[data];
		} else {
			regData = data;
		}

		//Performs the instruciton
		switch (opcode) {
			case JMP:
				programCounter = JUMP;
				break;
			case ADD:
				accumulator = mask(accumulator + regData);
				break;
			case SUB:
				accumulator = mask(accumulator - regData);
				break;
			case LSH:
				accumulator = mask(accumulator << regData);
				break;
			case RSH:
				accumulator = (accumulator >> regData);
				break;
			case AND:
				accumulator = (accumulator & regData);
				break;
			case OR:
				accumulator = (accumulator | regData);
				break;
			case XOR:
				accumulator = (accumulator ^ regData);
				break;
			case LDA:
				accumulator = regData;
				break;
			case STA:
				registers[data] = accumulator;
				break;
			case CND:
				if (conditionFlag) {
					programCounter = JUMP;
				}
				break;
			case PSH:
				stack_push(&jmpStack, programCounter);
				programCounter = JUMP;
				break;
			case POP:
				programCounter = stack_pop(&jmpStack) & BINARY_MASK;
				break;
			case CMP:
				conditionFlag = (mod & CMP_NOT_FLAG) && 1;
				if(((CMP_LT_FLAG & mod) && accumulator < regData) || ((CMP_EQ_FLAG & mod) && accumulator == regData)) {
					invert(conditionFlag);
				}
				break;
			case ICP:
				conditionFlag = (mod & CMP_NOT_FLAG) && 1;
				if(((CMP_LT_FLAG & mod) && accumulator < data) || ((CMP_EQ_FLAG & mod) && accumulator == data)) {
					invert(conditionFlag);
				}
				break;
			case STK:
				if (mod & STK_POP_FLAG) {
					set(stack_pop(&valStack));
				} else {
					stack_push(&valStack, regData);
				}
				break;
			case RPA:
				accumulator = mask(ports[mask(mod)].output);
				break;
			case RPR:
				registers[data] = ports[mask(mod)].output;
				break;
			case WPA:
				port_handlePort(CURRENT_PORT, accumulator);
				break;
			case WPR:
				port_handlePort(CURRENT_PORT, regData);
				break;
		}

		programCounter += INSTRUCTION_STEP;
	}

}

/// @brief Prints the program counter, accumulator, and all registers in a readable format
void printState() {
	clear_screen();
	printf("PC: %d\n", programCounter / INSTRUCTION_STEP);
	printf("ACC: %d\n", accumulator);
	for (int i = 0; i < REGISTERS_SIZE; i++) {
		printf("\t$%d: %d\n", i, registers[i]);
	}
}

/// @brief Prints the current state of the screen, which is stored in port 1
void printScreen() {
	clear_screen();
	char result[SCREEN_HEIGHT * (SCREEN_WIDTH * 2)];
	result[sizeof(result) - 1] = '\0';
	printf("%s", GPU_getScreen(result));
	printf("\n");
}

/// @brief Reads the entire file and set the binary to the contents of the file, which should be a compiled BPU program
/// @param filename The file being read, which should be a compiled BPU program
void readBinary(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (file) {
		fread(binary, 1, BINARY_SIZE, file);
		fclose(file);
	} else {
		printf("Error: Could not open file %s\n", filename);
	}
	
}

void speedTest() {
	clock_t start = clock();
	#define CYCLES 500000000
	exec(CYCLES);
	printf("%d Hz\n", CLOCKS_PER_SEC * ((long) CYCLES / (clock() - start)));
}

int main(int argc, char *argv[]) {
	init();
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
			readBinary(argv[i]);
		} else {
			switch (argv[i][1]) {
				case 's':
					speedTest();
					return 0;
				default:
					printf("Unknown flag: %s\n", argv[i]);
			}
		}
	}

	while(TRUE) {
		exec(240);
		sleepms(5);
		printScreen();
	}

	return 0;
}