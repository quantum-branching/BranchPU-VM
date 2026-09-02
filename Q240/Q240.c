#include <stdio.h>
#include <stdlib.h>

#include "Types240.h"
#include "Encoding240.h"
#include "../Port/Port.c"

u16 binary[MAX_INSTRUCTIONS];

u8 instructions[MAX_INSTRUCTIONS];
u8 source1[MAX_INSTRUCTIONS];
u8 source2[MAX_INSTRUCTIONS];
u8 destinations[MAX_INSTRUCTIONS];

/// +-----------+-------+-------+
/// | Reg(s)	| Src	| Dest	|
/// +-----------+-------+-------+
/// | r0		| 0		| 16	|
/// | r1-r15	| 1-15	| 1-15	|
/// | CND Flag	| 17	| 17	|
/// | Value SP	| 18	| 18	|
/// | Prog SP	| 19	| 19	|
/// +-----------+-------+-------+
u8 registers[VIRTUAL_REGS];
u8 RAM[RAM_SIZE];
u8 value_stack[RAM_SIZE];
u16 program_stack[PROG_STACK_COUNT];
u16 programCounter;

struct Port ports[16];

usize exec(const usize cycles) {
	u16 PC = programCounter;

	u8 dest = destinations[PC];
	u8 src1 = source1[PC];
	u8 src2 = source2[PC];

	for(usize i = 0; i < cycles; i++) {
		dest = destinations[PC];
		src1 = source1[PC];
		src2 = source2[PC];

		switch(instructions[PC]) {
			case ADD:
				registers[dest] = registers[src1] + registers[src2];
				break;
			case SUB:
				registers[dest] = registers[src1] - registers[src2];
				break;	
			case LSH:
				registers[dest] = registers[src1] << src2;
				break;
			case RSH:
				registers[dest] = registers[src1] >> src2;
				break;
			case LGC:
				switch(src2) {
					case 0:
						registers[dest] = 0;
						break;
					case 1:
						registers[dest] = ~ (registers[src1] | registers[dest]);
						break;
					case 2:
						registers[dest] ^= registers[src1];
						break;
					case 3:
						registers[dest] = ~ (registers[src1] & registers[dest]);
						break;
					case 4:
						registers[dest] &= registers[src1];
						break;
					case 5:
						registers[dest] = ~ (registers[src1] ^ registers[dest]);
						break;
					case 6:
						registers[dest] |=  registers[src1];
						break;
					case 7:
						registers[dest] = 255;
						break;
				}
			case LDI:
				registers[dest] = src1;
				break;
			case STK:
				switch(dest) {
					case 0:
						value_stack[registers[STACK_PTR]++] = registers[src1];
						break;
					case 1:
						value_stack[registers[STACK_PTR]++] = src1;
						break;
					case 2:
						registers[src1] = value_stack[--registers[STACK_PTR]];
						break;
					case 3:
						break;
				}
			case PEK:
				registers[dest] = value_stack[registers[src1]];
				break;
			case CMP:
				switch(dest) {
					case 0:
						registers[CND_FLAG] = 0;
						break;
					case 1:
						registers[CND_FLAG] = registers[src1] < registers[src2] ? 1 : 0;
						break;
					case 2:
						registers[CND_FLAG] = registers[src1] == registers[src2] ? 1 : 0;
						break;
					case 3:
						registers[CND_FLAG] = registers[src1] <= registers[src2] ? 1 : 0;
						break;
					case 4:
						registers[CND_FLAG] = 1;
						break;
					case 5:
						registers[CND_FLAG] = registers[src1] >= registers[src2] ? 1 : 0;
						break;
					case 6:
						registers[CND_FLAG] = registers[src1] != registers[src2] ? 1 : 0;
						break;
					case 7:
						registers[CND_FLAG] = registers[src1] > registers[src2] ? 1 : 0;
						break;
				}
				break;
			case JMP:
				if(src1 & 0x8) {

				} else {

				}
				break;
			case PSH:
				break;
			case POP:
				break;
			case LOD:
				break;
			case STR:
				break;
			case RPT:
				break;
			case WPT:
				break;
		}

		PC += 1;
	}

	programCounter = PC;
	return cycles;
}

void readBinary(const char *filename) {
	FILE *file = fopen(filename, "rb");

	if(file == NULL) {
		printf("Error: Could not open file %s\n", filename);
		return;
	}

	fread(binary, 2, MAX_INSTRUCTIONS, file);
	fclose(file);
}

u8 destination(u8 reg) {
	reg = (reg & NIB2) >> 8;
	return reg ? reg : 17;
}

void unpack() {
	for(u32 i = 0; i < MAX_INSTRUCTIONS; i++) {
		instructions[i] = binary[i] >> 12;
		switch(instructions[i]) {
			case 0x0:
				destinations[i] = destination(instructions[i]);
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = instructions[i] & NIB4;
			case 0x1:
				destinations[i] = destination(instructions[i]);
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = instructions[i] & NIB4;
			case 0x2:
				destinations[i] = destination(instructions[i]);
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = instructions[i] & NIB4;
			case 0x3:
				destinations[i] = destination(instructions[i]);
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = instructions[i] & NIB4;
			case 0x4:
				destinations[i] = (instructions[i] & NIB3) >> 4;
				source1[i] = instructions[i] & NIB4;
				source2[i] = (instructions[i] & 0x0700) >> 8;
			case 0x5:
				destinations[i] = destination(instructions[i]);
				source1[i] = instructions[i] & IMM;
				source2[i] = 0;
			case 0x6:
				destinations[i] = (instructions[i] & NIB2) >> 8;
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = instructions[i] & NIB4;
			case 0x7:
				destinations[i] = destination(instructions[i]);
				source1[i] = instructions[i] & NIB3;
				source2[i] = 0;
			case 0x8:
				destinations[i] = (instructions[i] & 0x0700) >> 8;
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = instructions[i] & NIB4;
			case 0x9:
				destinations[i] = 0;
				source1[i] = (instructions[i] & 0x0F00) >> 8;
				source2[i] = instructions[i] & IMM;
			case 0xA:
				destinations[i] = 0;
				source1[i] = (instructions[i] & 0x0F00) >> 8;
				source2[i] = instructions[i] & IMM;
			case 0xB:
				destinations[i] = (instructions[i] & 0x0700) >> 8;
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = instructions[i] & NIB4;
			case 0xC:
				destinations[i] = destination(instructions[i]);
				source1[i] = instructions[i] & IMM;
				source2[i] = 0;
			case 0xD:
				destinations[i] = instructions[i] & IMM;
				source1[i] = (instructions[i] & NIB2) >> 8;
				source2[i] = 0;
			case 0xE:
				destinations[i] = destination(instructions[i]);
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = 0;
			case 0xF:
				destinations[i] = (instructions[i] & NIB2) >> 8;
				source1[i] = (instructions[i] & NIB3) >> 4;
				source2[i] = 0;
			default:
				puts("Instruction not supported!");
		}
	}
}

int main(int argc, char **argv) {
	
}