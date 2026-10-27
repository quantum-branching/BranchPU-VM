#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Types240.h"
#include "Encoding240.h"

#include "../IO/Port.c"
#include "../IO/GPU.h"

u16 binary_240[MAX_INSTRUCTIONS__240];

u8 instructions__240[MAX_INSTRUCTIONS__240];
u8 source1__240[MAX_INSTRUCTIONS__240];
u8 source2_240[MAX_INSTRUCTIONS__240];
u8 destinations__240[MAX_INSTRUCTIONS__240];

// +------------+-------+-------+
// | Reg(s)		| Src	| Dest	|
// +------------+-------+-------+
// | r0			| 0		| 16	|
// | r1-r15		| 1-15	| 1-15	|
// | CND Flag	| 17	| 17	|
// | Value SP	| 18	| 18	|
// | Prog SP	| 19	| 19	|
// +------------+-------+-------+
u8 registers__240[VIRTUAL_REGS__240];
u8 RAM__240[RAM_SIZE__240];
u8 value_stack__240[RAM_SIZE__240];
u16 program_stack__240[PROG_STACK_COUNT__240];
u16 programCounter__240;

struct Port ports__240[16];

usize exec240(const usize cycles) {
	u16 PC = programCounter__240;

	u8 dest = destinations__240[PC];
	u8 src1 = source1__240[PC];
	u8 src2 = source2_240[PC];

	for(usize i = 0; i < cycles; i++) {
		dest = destinations__240[PC];
		src1 = source1__240[PC];
		src2 = source2_240[PC];

		switch(instructions__240[PC]) {
			case ADD__240:
				registers__240[dest] = registers__240[src1] + registers__240[src2];
				break;
			case SUB__240:
				registers__240[dest] = registers__240[src1] - registers__240[src2];
				break;	
			case LSH__240:
				registers__240[dest] = registers__240[src1] << src2;
				break;
			case RSH__240:
				registers__240[dest] = registers__240[src1] >> src2;
				break;
			case LGC__240:
				switch(src2) {
					case 0:
						registers__240[dest] = 0;
						break;
					case 1:
						registers__240[dest] = ~ (registers__240[src1] | registers__240[dest]);
						break;
					case 2:
						registers__240[dest] ^= registers__240[src1];
						break;
					case 3:
						registers__240[dest] = ~ (registers__240[src1] & registers__240[dest]);
						break;
					case 4:
						registers__240[dest] &= registers__240[src1];
						break;
					case 5:
						registers__240[dest] = ~ (registers__240[src1] ^ registers__240[dest]);
						break;
					case 6:
						registers__240[dest] |=  registers__240[src1];
						break;
					case 7:
						registers__240[dest] = 255;
						break;
				}
			case LDI__240:
				registers__240[dest] = src1;
				break;
			case STK__240:
				switch(dest) {
					case 0:
						push(value_stack__240, STACK_PTR__240, registers__240[src1]);
						break;
					case 1:
						push(value_stack__240, STACK_PTR__240, src1);
						break;
					case 2:
						pop(value_stack__240, STACK_PTR__240, registers__240[src1]);
						break;
					case 3:
						break;
				}
			case PEK__240:
				registers__240[dest] = value_stack__240[registers__240[src1]];
				break;
			case CMP__240:
				switch(dest) {
					case 0:
						CND_FLAG__240 = 0;
						break;
					case 1:
						CND_FLAG__240 = registers__240[src1] < registers__240[src2] ? 1 : 0;
						break;
					case 2:
						CND_FLAG__240 = registers__240[src1] == registers__240[src2] ? 1 : 0;
						break;
					case 3:
						CND_FLAG__240 = registers__240[src1] <= registers__240[src2] ? 1 : 0;
						break;
					case 4:
						CND_FLAG__240 = 1;
						break;
					case 5:
						CND_FLAG__240 = registers__240[src1] >= registers__240[src2] ? 1 : 0;
						break;
					case 6:
						CND_FLAG__240 = registers__240[src1] != registers__240[src2] ? 1 : 0;
						break;
					case 7:
						CND_FLAG__240 = registers__240[src1] > registers__240[src2] ? 1 : 0;
						break;
				}
				break;
			case JMP__240:
				if(src1 & 0x8) {
					if(CND_FLAG__240) {
						PC = (dest << 8) + src2 - 1;
					}
				} else {
					PC = (src1 << 8) + src2 - 1;
				}

				break;
			case PSH__240:
				push(program_stack__240, PC_STACK_PTR__240, PC);

				PC = (src1 << 8) + src2;
				PC -= 1;
				break;
			case POP__240:
				switch(dest) {
					case 0:
						programCounter__240 = PC;
						return i;
					case 1:
						if(registers__240[src1] < registers__240[src2]) {
							pop(program_stack__240, PC_STACK_PTR__240, PC);
						}
						break;
					case 2:
						if(registers__240[src1] == registers__240[src2]) {
							pop(program_stack__240, PC_STACK_PTR__240, PC);
						}
						break;
					case 3:
						if(registers__240[src1] <= registers__240[src2]) {
							pop(program_stack__240, PC_STACK_PTR__240, PC);
						}
						break;
					case 4:
						pop(program_stack__240, PC_STACK_PTR__240, PC);
						break;
					case 5:
						if(registers__240[src1] >= registers__240[src2]) {
							pop(program_stack__240, PC_STACK_PTR__240, PC);
						}
						break;
					case 6:
						if(registers__240[src1] != registers__240[src2]) {
							pop(program_stack__240, PC_STACK_PTR__240, PC);
						}
						break;
					case 7:
						if(registers__240[src1] > registers__240[src2]) {
							pop(program_stack__240, PC_STACK_PTR__240, PC);
						}
						break;
				}
				break;
			case LOD__240:
				registers__240[dest] = RAM__240[src1];
				break;
			case STR__240:
				RAM__240[dest] = registers__240[src1];
				break;
			case RPT__240:
				registers__240[dest] = ports__240[src1].output;
				break;
			case WPT__240:
				port_handlePort(ports__240[dest], registers__240[src1]);
				break;
		}

		PC += 1;
	}

	programCounter__240 = PC;
	return cycles;
}

void readBin240(const char *filename) {
	FILE *file = fopen(filename, "rb");

	if(file == NULL) {
		printf("Error: Could not open file %s\n", filename);
		return;
	}

	usize count = fread(binary_240, sizeof(*binary_240), MAX_INSTRUCTIONS__240, file);
	
	for(int i = 0; i < count; i++) {
		binary_240[i] = ((binary_240[i] & 0xFF) << 8) + ((binary_240[i] & 0xFF00) >> 8);
	}

	for(int i = count; i < MAX_INSTRUCTIONS__240; i++) {
		binary_240[i] = 0;
	}

	fclose(file);
}

u8 destination242(u16 reg) {
	reg = (reg & NIB2__240) >> 8;
	return reg ? reg : NULL_DEST__240;
}

void unpack240() {
	ports__240[1].update = GPU_p26;

	for(u32 i = 0; i < MAX_INSTRUCTIONS__240; i++) {
		instructions__240[i] = binary_240[i] >> 12;
		switch(instructions__240[i]) {
			case 0x0:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = binary_240[i] & NIB4__240;
				break;
			case 0x1:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = binary_240[i] & NIB4__240;
				break;
			case 0x2:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = binary_240[i] & NIB4__240;
				break;
			case 0x3:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = binary_240[i] & NIB4__240;
				break;
			case 0x4:
				destinations__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source1__240[i] = binary_240[i] & NIB4__240;
				source2_240[i] = (binary_240[i] & 0x0700) >> 8;
				break;
			case 0x5:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = binary_240[i] & IMM__240;
				source2_240[i] = 0;
				break;
			case 0x6:
				destinations__240[i] = (binary_240[i] & NIB2__240) >> 8;
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = binary_240[i] & NIB4__240;
				break;
			case 0x7:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = binary_240[i] & NIB3__240;
				source2_240[i] = 0;
				break;
			case 0x8:
				destinations__240[i] = (binary_240[i] & 0x0700) >> 8;
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = binary_240[i] & NIB4__240;
				break;
			case 0x9:
				destinations__240[i] = (binary_240[i] & 0x0700) >> 8;
				source1__240[i] = (binary_240[i] & 0x0F00) >> 8;
				source2_240[i] = binary_240[i] & IMM__240;
				break;
			case 0xA:
				destinations__240[i] = 0;
				source1__240[i] = (binary_240[i] & 0x0F00) >> 8;
				source2_240[i] = binary_240[i] & IMM__240;
				break;
			case 0xB:
				destinations__240[i] = (binary_240[i] & 0x0700) >> 8;
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = binary_240[i] & NIB4__240;
				break;
			case 0xC:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = binary_240[i] & IMM__240;
				source2_240[i] = 0;
				break;
			case 0xD:
				destinations__240[i] = binary_240[i] & IMM__240;
				source1__240[i] = (binary_240[i] & NIB2__240) >> 8;
				source2_240[i] = 0;
				break;
			case 0xE:
				destinations__240[i] = destination242(binary_240[i]);
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = 0;
				break;
			case 0xF:
				destinations__240[i] = (binary_240[i] & NIB2__240) >> 8;
				source1__240[i] = (binary_240[i] & NIB3__240) >> 4;
				source2_240[i] = 0;
				break;
			default:
				puts("Instruction not supported!");
		}
	}
}

void printState240() {
	for(int i = 0; i < 16; i++) {
		printf("r%X: %X\t", i, registers__240[i]);
		if(i % 4 == 3) {
			puts("");
		}
	}
	printf("PC: %03X\t\t", programCounter__240);
	printf("CND: %d\n", registers__240[17]);
	printf("Val SP: %02X\t", registers__240[18]);
	printf("Adr SP: %02X\n\n", registers__240[19]);
}

void speedTest240(char flag) {
	clock_t start = clock();
	#define CYCLES 500000000.0
	usize clocks = exec240((usize) CYCLES);
	double time = clock() - start;
	printf("%f Hz\n", CLOCKS_PER_SEC * (clocks / time));

	if(flag) {
		speedTest240(flag);
	}
}
