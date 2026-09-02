#define MAX_INSTRUCTIONS 4096
#define ROM_SIZE 8192

/// Number of user registers
#define MAX_REGS 16
#define MAX_PORTS 16

/// Internal registers
#define CND_FLAG 17
#define STACK_PTR 18
#define PC_STACK_PTR 19

/// User registers and internal registers
#define VIRTUAL_REGS 20

#define RAM_SIZE 256
#define PROG_STACK_COUNT 32
#define PROG_STACK_SIZE 48
#define VAL_STACK_SIZE 256

#define NIB1 0xF000
#define NIB2 0x0F00
#define NIB3 0x00F0
#define NIB4 0x000F

#define IMM 0x0FF

#define ADD 0x0
#define SUB 0x1
#define LSH 0x2
#define RSH 0x3
#define LGC 0x4
#define LDI 0x5
#define STK 0x6
#define PEK 0x7
#define CMP 0x8
#define JMP 0x9
#define CND 0x9
#define PSH 0xA
#define HLT 0xB
#define POP 0xB
#define LOD 0xC
#define STR 0xD
#define RPT 0xE
#define WPT 0xF