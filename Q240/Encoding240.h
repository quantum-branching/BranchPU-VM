#pragma once

#define MAX_INSTRUCTIONS__240 2048
#define ROM_SIZE__240 4096

/// Number of user registers
#define MAX_REGS__240 16
#define MAX_PORTS__240 16

/// Internal registers
#define NULL_DEST__240 16
#define CND_FLAG__240 (registers[17])
#define STACK_PTR__240 (registers[18])
#define PC_STACK_PTR__240 (registers[19])

/// User registers and internal registers
#define VIRTUAL_REGS__240 20

#define RAM_SIZE__240 256
#define PROG_STACK_COUNT__240 32
#define PROG_STACK_SIZE__240 48
#define VAL_STACK_SIZE__240 256

#define NIB1__240 0xF000
#define NIB2__240 0x0F00
#define NIB3__240 0x00F0
#define NIB4__240 0x000F

#define IMM__240 0x0FF

#define ADD__240 0x0
#define SUB__240 0x1
#define LSH__240 0x2
#define RSH__240 0x3
#define LGC__240 0x4
#define LDI__240 0x5
#define STK__240 0x6
#define PEK__240 0x7
#define CMP__240 0x8
#define JMP__240 0x9
#define CND__240 0x9
#define PSH__240 0xA
#define HLT__240 0xB
#define POP__240 0xB
#define LOD__240 0xC
#define STR__240 0xD
#define RPT__240 0xE
#define WPT__240 0xF