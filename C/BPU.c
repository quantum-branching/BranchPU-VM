#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ISA_SIZE 20
#define MAX_LINE_LENGTH 128
#define PROGRAM_SIZE 2048
#define REGISTERS 256

//The opcode for every instruction that is in the program
uint8_t instructions[PROGRAM_SIZE];
//The argument for every instruction that is in the program
uint8_t arguments[PROGRAM_SIZE];

//The instruction that is currently being executed (program counter)
int instruction;
//The state of the 256 byte memory
uint8_t registers[REGISTERS];
//The state of the accumulator - the value being modified by instructions
uint8_t accumulator;

bool startsWith(char text[], char sub[]) {
	int size = strlen(sub);
	int idx = 0;
	while(text[idx] == sub[idx]) {
		idx++;
		if(idx == size) {
			return true;
		}
	}
	return false;
}

void BPUcompile(char* filename) {
	//Instructions Set
	char isa[ISA_SIZE][4] = {"JMP", "ADD", "SUB", "LSH", "RSH", "AND", "OR", "XOR", "LDA", "STA", "CND", "PSH", "POP", "CMP", "ICP", "STK", "RPA", "RPR", "WPA", "WPR"};
	//File being read
	FILE *fptr;
	//Line being read
	char line[MAX_LINE_LENGTH];
	
	//Set Global Variables
	instruction = 0;
	
	//Opens the requested file
	fptr = fopen(filename, "r");
	
	//Reads requested file
	while(fgets(line, MAX_LINE_LENGTH, fptr)) {
		for(int i = 0; i < ISA_SIZE; i++) {
			if(startsWith(line,isa[i])) {
				instructions[instruction] = i;
				instruction++;
			}
		}
	}
	
	// Close the file
	fclose(fptr);
}

int main(int argc, char* argv[]) {
	BPUcompile(argv[1]);
	return 0;
}