#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Types240.h"
#include "Encoding240.h"
#include "../Port/Port.c"
#include "../Port/GPU.c"

u16 binary[MAX_INSTRUCTIONS];

u8 instructions[MAX_INSTRUCTIONS];
u8 source1[MAX_INSTRUCTIONS];
u8 source2[MAX_INSTRUCTIONS];
u8 destinations[MAX_INSTRUCTIONS];

// +------------+-------+-------+
// | Reg(s)		| Src	| Dest	|
// +------------+-------+-------+
// | r0			| 0		| 16	|
// | r1-r15		| 1-15	| 1-15	|
// | CND Flag	| 17	| 17	|
// | Value SP	| 18	| 18	|
// | Prog SP	| 19	| 19	|
// +------------+-------+-------+
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

		// printf("D: %02X (%02X)\t1: %02X (%02X)\t2: %02X (%02X)\tInstruction: %04X\tPC: %03X\n", dest, registers[dest], src1, registers[src1], src2, registers[src2], binary[PC], PC);
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
						push(value_stack, STACK_PTR, registers[src1]);
						break;
					case 1:
						push(value_stack, STACK_PTR, src1);
						break;
					case 2:
						pop(value_stack, STACK_PTR, registers[src1]);
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
						CND_FLAG = 0;
						break;
					case 1:
						CND_FLAG = registers[src1] < registers[src2] ? 1 : 0;
						break;
					case 2:
						CND_FLAG = registers[src1] == registers[src2] ? 1 : 0;
						break;
					case 3:
						CND_FLAG = registers[src1] <= registers[src2] ? 1 : 0;
						break;
					case 4:
						CND_FLAG = 1;
						break;
					case 5:
						CND_FLAG = registers[src1] >= registers[src2] ? 1 : 0;
						break;
					case 6:
						CND_FLAG = registers[src1] != registers[src2] ? 1 : 0;
						break;
					case 7:
						CND_FLAG = registers[src1] > registers[src2] ? 1 : 0;
						break;
				}
				break;
			case JMP:
				if(src1 & 0x8) {
					if(CND_FLAG) {
						PC = (dest << 8) + src2 - 1;
					}
				} else {
					PC = (src1 << 8) + src2 - 1;
				}

				break;
			case PSH:
				push(program_stack, PC_STACK_PTR, PC);

				PC = (src1 << 8) + src2;
				PC -= 1;
				break;
			case POP:
				switch(dest) {
					case 0:
						return i;
					case 1:
						if(registers[src1] < registers[src2]) {
							pop(program_stack, PC_STACK_PTR, PC);
						}
					case 2:
						if(registers[src1] == registers[src2]) {
							pop(program_stack, PC_STACK_PTR, PC);
						}
						break;
					case 3:
						if(registers[src1] <= registers[src2]) {
							pop(program_stack, PC_STACK_PTR, PC);
						}
						break;
					case 4:
						pop(program_stack, PC_STACK_PTR, PC);
						break;
					case 5:
						if(registers[src1] >= registers[src2]) {
							pop(program_stack, PC_STACK_PTR, PC);
						}
						break;
					case 6:
						if(registers[src1] != registers[src2]) {
							pop(program_stack, PC_STACK_PTR, PC);
						}
						break;
					case 7:
						if(registers[src1] > registers[src2]) {
							pop(program_stack, PC_STACK_PTR, PC);
						}
						break;
				}
				break;
			case LOD:
				registers[dest] = RAM[src1];
				break;
			case STR:
				RAM[dest] = registers[src1];
				break;
			case RPT:
				registers[dest] = ports[src1].output;
				break;
			case WPT:
				port_handlePort(ports[dest], registers[src1]);
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

	usize count = fread(binary, sizeof(*binary), MAX_INSTRUCTIONS, file);
	
	for(int i = 0; i < count; i++) {
		binary[i] = ((binary[i] & 0xFF) << 8) + ((binary[i] & 0xFF00) >> 8);
	}

	for(int i = count; i < MAX_INSTRUCTIONS; i++) {
		binary[i] = 0;
	}

	fclose(file);
}

u8 destination(u16 reg) {
	reg = (reg & NIB2) >> 8;
	return reg ? reg : NULL_DEST;
}

void unpack() {
	ports[1].update = GPU_p26;

	for(u32 i = 0; i < MAX_INSTRUCTIONS; i++) {
		instructions[i] = binary[i] >> 12;
		switch(instructions[i]) {
			case 0x0:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = binary[i] & NIB4;
				break;
			case 0x1:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = binary[i] & NIB4;
				break;
			case 0x2:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = binary[i] & NIB4;
				break;
			case 0x3:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = binary[i] & NIB4;
				break;
			case 0x4:
				destinations[i] = (binary[i] & NIB3) >> 4;
				source1[i] = binary[i] & NIB4;
				source2[i] = (binary[i] & 0x0700) >> 8;
				break;
			case 0x5:
				destinations[i] = destination(binary[i]);
				source1[i] = binary[i] & IMM;
				source2[i] = 0;
				break;
			case 0x6:
				destinations[i] = (binary[i] & NIB2) >> 8;
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = binary[i] & NIB4;
				break;
			case 0x7:
				destinations[i] = destination(binary[i]);
				source1[i] = binary[i] & NIB3;
				source2[i] = 0;
				break;
			case 0x8:
				destinations[i] = (binary[i] & 0x0700) >> 8;
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = binary[i] & NIB4;
				break;
			case 0x9:
				destinations[i] = (binary[i] & 0x0700) >> 8;
				source1[i] = (binary[i] & 0x0F00) >> 8;
				source2[i] = binary[i] & IMM;
				break;
			case 0xA:
				destinations[i] = 0;
				source1[i] = (binary[i] & 0x0F00) >> 8;
				source2[i] = binary[i] & IMM;
				break;
			case 0xB:
				destinations[i] = (binary[i] & 0x0700) >> 8;
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = binary[i] & NIB4;
				break;
			case 0xC:
				destinations[i] = destination(binary[i]);
				source1[i] = binary[i] & IMM;
				source2[i] = 0;
				break;
			case 0xD:
				destinations[i] = binary[i] & IMM;
				source1[i] = (binary[i] & NIB2) >> 8;
				source2[i] = 0;
				break;
			case 0xE:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = 0;
				break;
			case 0xF:
				destinations[i] = (binary[i] & NIB2) >> 8;
				source1[i] = (binary[i] & NIB3) >> 4;
				source2[i] = 0;
				break;
			default:
				puts("Instruction not supported!");
		}
	}
}

void printScreen() {
	char result[SCREEN_HEIGHT * (SCREEN_WIDTH * 2)];
	GPU_getScreen(result);
	result[sizeof(result) - 1] = '\0';
	puts(result);
}

void printState() {
	for(int i = 0; i < 16; i++) {
		printf("r%X: %X\t", i, registers[i]);
		if(i % 4 == 3) {
			puts("");
		}
	}
}

void speedTest(char flag) {
	clock_t start = clock();
	#define CYCLES 500000000.0
	usize clocks = exec((usize) CYCLES);
	double time = clock() - start;
	printf("%f Hz\n", CLOCKS_PER_SEC * (clocks / time));

	if(flag) {
		speedTest(flag);
	}
}


int main(int argc, char **argv) {
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
			readBinary(argv[i]);
			unpack();
		} else {
			switch (argv[i][1]) {
				case 'a':
					puts("Assembler not complete");
					return 0;
				case 's':
					speedTest(argv[i][2]);
					return 0;
				case 'v':
					puts("BranchPU VM v0.7, Copyright (C) 2026 QuantumBranching");
					return 0;
				default:
					printf("Unknown flag: %s\n", argv[i]);
			}
		}
	}

	for(;;) {
		exec(200);
		printScreen();
	}
}