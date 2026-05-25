/// @file Encoding.c
/// @brief The encoding for the BPU, which defines the format of the instructions and how to decode them

/* ------------------------- Size ------------------------- */

///Maximum binary size, 4096 B (4 KB) for 2048 
///instructions (2 B each)
#define BINARY_SIZE 4096
///Total number of registers, 256 for 8-bit register 
///addressing
#define REGISTERS_SIZE 256
///Total number of ports, 8 for 3-bit port addressing
#define PORTS_SIZE 8

/* ------------------------- Range ------------------------- */

///The number of possible values for a byte, which is 256 for 
///an 8-bit value
#define BYTE_RANGE 256
///The number of possible values for a modifier, which is 8 
///for a 3-bit value
#define MOD_RANGE 8
///The number of possible values for a double operand, which 
///is 2048 for an 11-bit value (mod + byte)
#define DOUBLE_RANGE 2048

/* ------------------------- Masks ------------------------- */

///The maximum value the program counter can reach
#define BINARY_MASK 4095
///Limits the range for which a byte can be in
#define BYTE_MASK 255
///The mask for the 5-bit opcode in the instruction
#define INSTRUCTION_MASK 31
///The mask for the 3-bit modifier in the instruction
#define MOD_MASK 7

/* ------------------------- Offsets ------------------------- */

///The effect of the 3-bit modifier when added to the 8-bit 
///operand (for an 11-bit value)
#define MOD_OFFSET 8
///The offset of the instruction opcode from the lowest bit 
///of the a given instruction byte
#define INSTRUCTION_OFFSET 3
///Offset from the instruction for which the data lies
#define DATA_OFFSET 1

/* -------------------------- Steps -------------------------- */

///The distance the CPU must step for a new instruction
#define INSTRUCTION_STEP 2

/* -------------------------- Flags -------------------------- */

///The flag for an immediate operand in the modifier
#define IMMEDIATE_FLAG 1
///The flag for a less than condition in the modifier for the CMP 
///and ICP instructions
#define CMP_LT_FLAG 1
///The flag for an equal to condition in the modifier for the CMP 
///and ICP instructions
#define CMP_EQ_FLAG 2
///The flag for a not condition in the modifier for the CMP and 
///ICP instructions
#define CMP_NOT_FLAG 4

///The flag for popping the stack in the modifier for the STK 
///instruction
#define STK_POP_FLAG 2