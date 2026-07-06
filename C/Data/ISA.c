/// @file ISA.c
/// @brief The instruction set architecture for the BPU, which defines the opcodes and their effects on the CPU state

///Jumps to an 11-bit address using the modifier as the high bits and the operand as the low bits
#define JMP 0x00
///Sets the accumulator to the sum of the accumulator and a value
#define ADD 0x01
///Sets the accumulator to the difference of the accumulator and a value
#define SUB 0x02
///Left shifts the accumulator by a value
#define LSH 0x03
///Right shifts the accumulator by a value
#define RSH 0x04
///Sets the accumulator to the bitwise AND of the accumulator and a value
#define AND 0x05
///Sets the accumulator to the bitwise OR of the accumulator and a value
#define OR  0x06
///Sets the accumulator to the bitwise XOR of the accumulator and a value
#define XOR 0x07
///Load Accumulator from Register
#define LDA 0x08
///Load Accumulator from Register
#define LDI 0x28
///Store Accumulator to Register
#define STA 0x09
///Conditional Jump
#define CND 0x0A
///Push to Program Counter Stack
#define PSH 0x0B
///Pop from Program Counter Stack
#define POP 0x0C
///Compare Accumulator with Register to Condition Flag
#define CMP 0x0D
///Immediate Comparison
#define ICP 0x0E
///Value Stack
#define STK 0x0F
///Read Port to Accumulator
#define RPA 0x10
///Read Port to Register
#define RPR 0x11
///Write Port from Accumulator
#define WPA 0x12
///Write Port from Register
#define WPR 0x13
///Peek at a value in the Value Stack
#define PEK 0x14

#define STRING_JMP 0x1BCA
#define STRING_ADD 0x0310
#define STRING_SUB 0x3450
#define STRING_LSH 0x21A6
#define STRING_RSH 0x317E
#define STRING_AND 0x0414
#define STRING_OR  0x2950
#define STRING_XOR 0x40F8
#define STRING_LDA 0x2019
#define STRING_LDI 0x2021
#define STRING_STA 0x3435
#define STRING_CND 0x095C
#define STRING_PSH 0x2C36
#define STRING_POP 0x2BD6
#define STRING_CMP 0x094E
#define STRING_ICP 0x1822
#define STRING_STK 0x343F
#define STRING_RPA 0x3129
#define STRING_RPR 0x313A
#define STRING_WPA 0x3E5D
#define STRING_WPR 0x3E6E
#define STRING_PEK 0x2ACD