#include <stdio.h>
#include "Types/String.c"
#include "Types/Bool.c"

#include "Data/ISA.c"
#include "Data/Encoding.c"

#define PROGRAM_SIZE 32768
#define MAX_LINE_LENGTH 256

char program[PROGRAM_SIZE];
/// @brief Preprocessed program that has no directives or labels 
char processed[PROGRAM_SIZE];
char binary[BINARY_SIZE];

/// @brief Reads the assembly file and converts it to binary
/// @param filename The file that is being converted from assembly to binary
void readAssembly(const char *filename) {
	FILE *file = fopen(filename, "r");
	if (file) {
		fread(program, 1, PROGRAM_SIZE, file);
		int lines = count(program, '\n') + 1;
		int removeBlank = 0;

		for (int i = 0; i < lines; i++) {
			char line[MAX_LINE_LENGTH] = {0};
			getSlice(program, '\n', i, line);
			char command[MAX_LINE_LENGTH] = {0};
			getSlice(line, ' ', 0, command);
		}

		fclose(file);
	} else {
		printf("Error: Could not open file %s\n", filename);
	}
}

/// @brief Converts a unary or binary operand to modifier operand and main operand, depending on the format of the operand
/// @param first The original operand, which may be in the format of either a unary operand or a binary operand with a space in between
/// @return 
int double_operand(const char *first) {
	char second[MAX_LINE_LENGTH];

	char firs[MAX_LINE_LENGTH];

	if (containChar(first, ' ')) {
		split(first, ' ', firs, second);
		return toInteger(firs, MOD_RANGE) << MOD_OFFSET | toByte(second);
	} else {
		return toInteger(first, DOUBLE_RANGE);
	}
}

/// @brief 
/// @param opcode The opcode of this instruction
/// @param first instruction arguments
/// @return Returns the lower byte of an instructions which contains an 8-bit register address or immediate
int operand(const int opcode, const char *first) {
	if (opcode == PSH || opcode == CMP || opcode == STK || opcode == RPA || opcode == RPR || opcode == WPA || opcode == WPR) {
		return double_operand(first) & BYTE_MASK;
	}
	return toInteger(first, BYTE_RANGE);
}


/// @brief Calculates the upper byte of a given instruction
/// @param operation The assembly instruction used to determine the opcode and modifier
/// @param first A string that will be modified to contains the instruction arguments
/// @return Returns the upper byte of an instructions which contains the 5-bit opcode and the 3-bit modifier
int opcode(const char *operation, char *first) {
	#define IMMEDIATE_PREDICATE (firs[0] == '#')
	#define IMMEDIATE_FLAG 1
	#define DOUBLE_MOD (double_operand(firs) >> 8)

	char op[MAX_LINE_LENGTH];
	char firs[MAX_LINE_LENGTH];
	char second[MAX_LINE_LENGTH];
	split(operation, ' ', op, firs);

	strncpy(first, firs, MAX_LINE_LENGTH);

	if(op[0] == '\0') {
		strncpy(first, "0", 1);
		return 1 << INSTRUCTION_OFFSET;
	} else if(strcmp(op, "JMP") == 0) {
		return 0 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else if(strcmp(op, "ADD") == 0) {
		return 1 << INSTRUCTION_OFFSET | IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "SUB") == 0) {
		return 2 << INSTRUCTION_OFFSET | IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "LSH") == 0) {
		return 3 << INSTRUCTION_OFFSET | IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "RSH") == 0) {
		return 4 << INSTRUCTION_OFFSET | IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "AND") == 0) {
		return 5 << INSTRUCTION_OFFSET | IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "OR") == 0) {
		return 6 << INSTRUCTION_OFFSET | IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "XOR") == 0) {
		return 7 << INSTRUCTION_OFFSET | IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "LDA") == 0) {
		return 8 << INSTRUCTION_OFFSET;
	} else if(strcmp(op, "LDI") == 0) {
		return 8 << INSTRUCTION_OFFSET | IMMEDIATE_FLAG;
	} else if(strcmp(op, "STA") == 0) {
		return 9 << INSTRUCTION_OFFSET;
	} else if(strcmp(op, "CND") == 0) {
		return 10 << INSTRUCTION_OFFSET + 4 * IMMEDIATE_PREDICATE;
	} else if(strcmp(op, "PSH") == 0) {
		return 11 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else if(strcmp(op, "POP") == 0) {
		return 12 << INSTRUCTION_OFFSET;
	} else if(strcmp(op, "CMP") == 0) {
		return 13 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else if(strcmp(op, "ICP") == 0) {
		return 14 << INSTRUCTION_OFFSET | IMMEDIATE_FLAG;
	} else if(strcmp(op, "STK") == 0) {
		return 15 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else if(strcmp(op, "RPA") == 0) {
		return 16 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else if(strcmp(op, "RPR") == 0) {
		return 17 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else if(strcmp(op, "WPA") == 0) {
		return 18 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else if(strcmp(op, "WPR") == 0) {
		return 19 << INSTRUCTION_OFFSET | DOUBLE_MOD;
	} else {
		printf("Error: Unknown operation %s\n", operation);
		return -1;
	}
	return 0;
}

void main(int argc, char *argv[]) {
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
			readAssembly(argv[i]);
		}
	}
}