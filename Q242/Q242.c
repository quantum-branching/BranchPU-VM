#include "Q242.h"
#include "Encoding242.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../Types/Stack.c"
#include "../Types/Bool.c"
#include "../IO/Port.c"
#include "../IO/GPU.h"
#include "../Q240/Types240.h"

/// Program Counter
u16 PC__242;
/// Carry Flag
u8 CF__242;
/// Zero Flag
u8 ZF__242;

u8 binary__242[BINARY_SIZE__242];
u8 RAM__242[RAM_SIZE__242];
u8 registers__242[REG_COUNT__242 + 1];

void init242() {

}

void readBin242(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (file) {
		fread(binary__242, 1, BINARY_SIZE__242, file);
		fclose(file);
	} else {
		printf("Error: Could not open file %s\n", filename);
	}
	
}

inline u8 destination242(u8 destNibble) {
	return destNibble ? destNibble : 16;
}

#define regs registers__242
#define binary binary__242

#define byte0 binary[PC]
#define byte1 binary[PC + 1]

#define setReg(destNibble, value) regs[(destNibble) ? (destNibble) : REG_COUNT__242] = value

usize exec242(const size_t cycles) {
	u16 PC = PC__242;

	u8 nibble0;
	u8 nibble1;
	u8 nibble2;
	u8 nibble3;

	for(usize i = 0; i < cycles; i++) {
		nibble0 = byte0 && 0xF0;
		nibble1 = byte0 && 0x0F;
		nibble2 = byte1 && 0xF0;
		nibble3 = byte1 && 0x0F;

		switch(nibble0) {
			case ADD__242:
				setReg(nibble1, regs[nibble2] + regs[nibble3]);
				break;
			case SUB__242:
				setReg(nibble1, regs[nibble2] - regs[nibble3]);
				break;
			case ADI__242:
				setReg(nibble1, regs[nibble1] + byte1);
				break;
			case LDI__242:
				setReg(nibble1, byte1);
				break;
			case BIT__242:
				u8 xor = regs[nibble1] ^ regs[nibble3];
				u8 ior = regs[nibble1] | regs[nibble3];
				u8 nor = ~ior;

				u8 ior_flag = 0 - ((nibble2 >> 1) & 0x1);
				u8 nor_flag = 0 - ((nibble2 >> 2) & 0x1);
				u8 inv_flag = 0 - (nibble2 & 0x1);

				setReg(nibble1, (xor | (ior & ior_flag) | (nor & nor_flag)) ^ inv_flag);
				break;
			case BRH__242:
				PC = ((((ZF__242 << 1) | CF__242) != (nibble2 & 0x3)) == !(nibble2 & 0x4)) ? (regs[nibble1] - 1) << 1 : PC;
    			break;
			case CAL__242:
				break;
			case RET__242:
				break;
			case ACH__242:
				break;
			case SCH__242:
				break;
			case RCH__242:
				break;
			case WCH__242:
				break;
			default:
				break;
		}
	}
}

#undef setReg

#undef binary
#undef regs
