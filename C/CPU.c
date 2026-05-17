#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "Types/Stack.c"
#include "Types/Bool.c"
#include "IO/Port.c"
#include "IO/GPU.c"

#define CLEAR_SCREEN "cls"

///Maximum binary size, 4096 B (4 KB) for 2048 instructions (2 B each)
#define BINARY_SIZE 4096
///Total number of registers, 256 for 8-bit register addressing
#define REGISTERS_SIZE 256
///Total number of ports, 8 for 3-bit port addressing
#define PORTS_SIZE 8

///The maximum value the program counter can reach
#define INSTRUCTION_LIMIT 4095
///The distance the CPU must step for a new instruction
#define INSTRUCTION_STEP 2
///The effect of the 3-bit modifier when added to the 8-bit operand (for an 11-bit value)
#define MOD_OFFSET 8
///The offset of the instruction opcode from the lowest bit of the a given instruction byte
#define INSTRUCTION_OFFSET 3
///The mask for the 5-bit opcode in the instruction
#define INSTRUCTION_MASK 31
///The mask for the 3-bit modifier in the instruction
#define MOD_MASK 7
///Offset from the instruction for which the data lies
#define DATA_OFFSET 1
///Limits the range for which a byte can be in
#define BYTE_MASK 255

///Jumps to an 11-bit address using the modifier as the high bits and the operand as the low bits
#define JMP 0
///Sets the accumulator to the sum of the accumulator and a value
#define ADD 1
///Sets the accumulator to the difference of the accumulator and a value
#define SUB 2
///Left shifts the accumulator by a value
#define LSH 3
///Right shifts the accumulator by a value
#define RSH 4
///Sets the accumulator to the bitwise AND of the accumulator and a value
#define AND 5
///Sets the accumulator to the bitwise OR of the accumulator and a value
#define OR  6
///Sets the accumulator to the bitwise XOR of the accumulator and a value
#define XOR 7
///Load Accumulator from Register
#define LDA 8
///Store Accumulator to Register
#define STA 9
///Conditional Jump
#define CND 10
///Push to Program Counter Stack
#define PSH 11
///Pop from Program Counter Stack
#define POP 12
///Compare Accumulator with Register to Condition Flag
#define CMP 13
///Immediate Comparison
#define ICP 14
///Value Stack
#define STK 15
///Read Port to Accumulator
#define RPA 16
///Read Port to Register
#define RPR 17
///Write Port from Accumulator
#define WPA 18
///Write Port from Register
#define WPR 19

/// The binary program loaded into the CPU, which can hold up to 4096 bytes of instructions and data
uint8_t binary[BINARY_SIZE];

/// The current program counter, which points to the current instruction in the binary
int programCounter;

/// The state of the CPU's registers, which can hold any value from 0 to 255
int registers[REGISTERS_SIZE];

/// The accumulator, which can hold any value from 0 to 255
int accumulator;

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

	ports[1].func = GPU_p26;
}

/// @brief Executes a given number of cycles of the loaded binary
/// @param cycles The number of cycles to execute
void exec(int cycles) {
	#define IMMEDIATE_FLAG 1
	#define CMP_LT_FLAG 1
	#define CMP_EQ_FLAG 2
	#define CMP_NOT_FLAG 4
	#define STK_POP_FLAG 2

	#define JUMP (INSTRUCTION_STEP * ((mod << MOD_OFFSET) | data) - INSTRUCTION_STEP) & INSTRUCTION_LIMIT
	#define CURRENT_PORT ports[mod & MOD_MASK]

	/// Sets the accumulator to a value, ensuring that it stays within the bounds of a byte
	#define accum(value) accumulator = (value) & BYTE_MASK

	/// Sets either the accumulator or a register to a value, depending on the state of the immediate flag in the modifier
	#define set(value) if(mod & IMMEDIATE_FLAG) { accumulator = (value) & BYTE_MASK; } else { registers[data] = (value) & BYTE_MASK; }

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
				accum(accumulator + regData);
				break;
			case SUB:
				accum(accumulator - regData);
				break;
			case LSH:
				accum(accumulator << regData);
				break;
			case RSH:
				accum(accumulator >> regData);
				break;
			case AND:
				accum(accumulator & regData);
				break;
			case OR:
				accum(accumulator | regData);
				break;
			case XOR:
				accum(accumulator ^ regData);
				break;
			case LDA:
				accum(regData);
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
				push(&jmpStack, programCounter);
				programCounter = JUMP;
				break;
			case POP:
				programCounter = pop(&jmpStack) & INSTRUCTION_LIMIT;
				break;
			case CMP:
				conditionFlag = (mod & CMP_NOT_FLAG) && 1;
				if(((CMP_LT_FLAG & mod) && accumulator < regData) || ((CMP_LT_FLAG & mod) && accumulator == regData)) {
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
					set(pop(&valStack));
				} else {
					push(&valStack, regData);
				}
				break;
			case RPA:
				accum(ports[mod & MOD_MASK].output);
				break;
			case RPR:
				registers[data] = ports[mod & MOD_MASK].output;
				break;
			case WPA:
				handlePort(CURRENT_PORT, accumulator);
				break;
			case WPR:
				handlePort(CURRENT_PORT, regData);
				break;
		}

		programCounter += INSTRUCTION_STEP;
	}

}

/// @brief Prints the program counter, accumulator, and all registers in a readable format
void printState() {
	system(CLEAR_SCREEN);
	printf("PC: %d\n", programCounter / INSTRUCTION_STEP);
	printf("ACC: %d\n", accumulator);
	for (int i = 0; i < REGISTERS_SIZE; i++) {
		printf("\t$%d: %d\n", i, registers[i]);
	}
}

/// @brief Prints the current state of the screen, which is stored in port 1
void printScreen() {
	system(CLEAR_SCREEN);
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 32; x++) {
			printf("%c ", getPixel(x, y) ? '#' : ' ');
		}
		printf("\n");
	}
}

/// @brief Reads the entire file and set the binary to the contents of the file, which should be a compiled BPU program
/// @param filename The file being read, which should be a compiled BPU program
void readFile(char *filename) {
	FILE *file = fopen(filename, "rb");
	if (file) {
		fread(binary, 1, BINARY_SIZE, file);
		fclose(file);
	} else {
		printf("Error: Could not open file %s\n", filename);
	}
	
}

int main(int argc, char *argv[]) {
	init();
	readFile(argv[1]);

	while(TRUE) {
		exec(60);
		printScreen();
	}

	return 0;
}
