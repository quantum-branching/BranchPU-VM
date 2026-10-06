#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Types240.h"
#include "Encoding240.h"

#include "../IO/Port.c"
#include "../IO/GPU.h"

#define binary binary__240
#define instructions instructions__240
#define source1 source1__240
#define source2 source2__240
#define destinations destinations__240
#define registers registers__240
#define RAM RAM__240
#define value_stack value_stack__240
#define program_stack program_stack__240
#define programCounter programCounter__240
#define ports ports__240

u16 binary[MAX_INSTRUCTIONS__240];

u8 instructions[MAX_INSTRUCTIONS__240];
u8 source1[MAX_INSTRUCTIONS__240];
u8 source2[MAX_INSTRUCTIONS__240];
u8 destinations[MAX_INSTRUCTIONS__240];

// +------------+-------+-------+
// | Reg(s)		| Src	| Dest	|
// +------------+-------+-------+
// | r0			| 0		| 16	|
// | r1-r15		| 1-15	| 1-15	|
// | CND Flag	| 17	| 17	|
// | Value SP	| 18	| 18	|
// | Prog SP	| 19	| 19	|
// +------------+-------+-------+
u8 registers[VIRTUAL_REGS__240];
u8 RAM[RAM_SIZE__240];
u8 value_stack[RAM_SIZE__240];
u16 program_stack[PROG_STACK_COUNT__240];
u16 programCounter;

struct Port ports[16];

usize exec240(const usize cycles) {
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
			case ADD__240:
				registers[dest] = registers[src1] + registers[src2];
				break;
			case SUB__240:
				registers[dest] = registers[src1] - registers[src2];
				break;	
			case LSH__240:
				registers[dest] = registers[src1] << src2;
				break;
			case RSH__240:
				registers[dest] = registers[src1] >> src2;
				break;
			case LGC__240:
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
			case LDI__240:
				registers[dest] = src1;
				break;
			case STK__240:
				switch(dest) {
					case 0:
						push(value_stack, STACK_PTR__240, registers[src1]);
						break;
					case 1:
						push(value_stack, STACK_PTR__240, src1);
						break;
					case 2:
						pop(value_stack, STACK_PTR__240, registers[src1]);
						break;
					case 3:
						break;
				}
			case PEK__240:
				registers[dest] = value_stack[registers[src1]];
				break;
			case CMP__240:
				switch(dest) {
					case 0:
						CND_FLAG__240 = 0;
						break;
					case 1:
						CND_FLAG__240 = registers[src1] < registers[src2] ? 1 : 0;
						break;
					case 2:
						CND_FLAG__240 = registers[src1] == registers[src2] ? 1 : 0;
						break;
					case 3:
						CND_FLAG__240 = registers[src1] <= registers[src2] ? 1 : 0;
						break;
					case 4:
						CND_FLAG__240 = 1;
						break;
					case 5:
						CND_FLAG__240 = registers[src1] >= registers[src2] ? 1 : 0;
						break;
					case 6:
						CND_FLAG__240 = registers[src1] != registers[src2] ? 1 : 0;
						break;
					case 7:
						CND_FLAG__240 = registers[src1] > registers[src2] ? 1 : 0;
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
				push(program_stack, PC_STACK_PTR__240, PC);

				PC = (src1 << 8) + src2;
				PC -= 1;
				break;
			case POP__240:
				switch(dest) {
					case 0:
						programCounter = PC;
						return i;
					case 1:
						if(registers[src1] < registers[src2]) {
							pop(program_stack, PC_STACK_PTR__240, PC);
						}
						break;
					case 2:
						if(registers[src1] == registers[src2]) {
							pop(program_stack, PC_STACK_PTR__240, PC);
						}
						break;
					case 3:
						if(registers[src1] <= registers[src2]) {
							pop(program_stack, PC_STACK_PTR__240, PC);
						}
						break;
					case 4:
						pop(program_stack, PC_STACK_PTR__240, PC);
						break;
					case 5:
						if(registers[src1] >= registers[src2]) {
							pop(program_stack, PC_STACK_PTR__240, PC);
						}
						break;
					case 6:
						if(registers[src1] != registers[src2]) {
							pop(program_stack, PC_STACK_PTR__240, PC);
						}
						break;
					case 7:
						if(registers[src1] > registers[src2]) {
							pop(program_stack, PC_STACK_PTR__240, PC);
						}
						break;
				}
				break;
			case LOD__240:
				registers[dest] = RAM[src1];
				break;
			case STR__240:
				RAM[dest] = registers[src1];
				break;
			case RPT__240:
				registers[dest] = ports[src1].output;
				break;
			case WPT__240:
				port_handlePort(ports[dest], registers[src1]);
				break;
		}

		PC += 1;
	}

	programCounter = PC;
	return cycles;
}

void readBin240(const char *filename) {
	FILE *file = fopen(filename, "rb");

	if(file == NULL) {
		printf("Error: Could not open file %s\n", filename);
		return;
	}

	usize count = fread(binary, sizeof(*binary), MAX_INSTRUCTIONS__240, file);
	
	for(int i = 0; i < count; i++) {
		binary[i] = ((binary[i] & 0xFF) << 8) + ((binary[i] & 0xFF00) >> 8);
	}

	for(int i = count; i < MAX_INSTRUCTIONS__240; i++) {
		binary[i] = 0;
	}

	fclose(file);
}

u8 destination(u16 reg) {
	reg = (reg & NIB2__240) >> 8;
	return reg ? reg : NULL_DEST__240;
}

void unpack240() {
	ports[1].update = GPU_p26;

	for(u32 i = 0; i < MAX_INSTRUCTIONS__240; i++) {
		instructions[i] = binary[i] >> 12;
		switch(instructions[i]) {
			case 0x0:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = binary[i] & NIB4__240;
				break;
			case 0x1:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = binary[i] & NIB4__240;
				break;
			case 0x2:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = binary[i] & NIB4__240;
				break;
			case 0x3:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = binary[i] & NIB4__240;
				break;
			case 0x4:
				destinations[i] = (binary[i] & NIB3__240) >> 4;
				source1[i] = binary[i] & NIB4__240;
				source2[i] = (binary[i] & 0x0700) >> 8;
				break;
			case 0x5:
				destinations[i] = destination(binary[i]);
				source1[i] = binary[i] & IMM__240;
				source2[i] = 0;
				break;
			case 0x6:
				destinations[i] = (binary[i] & NIB2__240) >> 8;
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = binary[i] & NIB4__240;
				break;
			case 0x7:
				destinations[i] = destination(binary[i]);
				source1[i] = binary[i] & NIB3__240;
				source2[i] = 0;
				break;
			case 0x8:
				destinations[i] = (binary[i] & 0x0700) >> 8;
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = binary[i] & NIB4__240;
				break;
			case 0x9:
				destinations[i] = (binary[i] & 0x0700) >> 8;
				source1[i] = (binary[i] & 0x0F00) >> 8;
				source2[i] = binary[i] & IMM__240;
				break;
			case 0xA:
				destinations[i] = 0;
				source1[i] = (binary[i] & 0x0F00) >> 8;
				source2[i] = binary[i] & IMM__240;
				break;
			case 0xB:
				destinations[i] = (binary[i] & 0x0700) >> 8;
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = binary[i] & NIB4__240;
				break;
			case 0xC:
				destinations[i] = destination(binary[i]);
				source1[i] = binary[i] & IMM__240;
				source2[i] = 0;
				break;
			case 0xD:
				destinations[i] = binary[i] & IMM__240;
				source1[i] = (binary[i] & NIB2__240) >> 8;
				source2[i] = 0;
				break;
			case 0xE:
				destinations[i] = destination(binary[i]);
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = 0;
				break;
			case 0xF:
				destinations[i] = (binary[i] & NIB2__240) >> 8;
				source1[i] = (binary[i] & NIB3__240) >> 4;
				source2[i] = 0;
				break;
			default:
				puts("Instruction not supported!");
		}
	}
}

void printState240() {
	for(int i = 0; i < 16; i++) {
		printf("r%X: %X\t", i, registers[i]);
		if(i % 4 == 3) {
			puts("");
		}
	}
	printf("PC: %03X\t\t", programCounter);
	printf("CND: %d\n", registers[17]);
	printf("Val SP: %02X\t", registers[18]);
	printf("Adr SP: %02X\n", registers[19]);
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


#undef binary
#undef instructions
#undef source1
#undef source2
#undef destinations
#undef registers
#undef RAM
#undef value_stack
#undef program_stack
#undef programCounter
#undef ports