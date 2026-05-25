/// @file ISA.c
/// @brief The instruction set architecture for the BPU, which defines the opcodes and their effects on the CPU state

///Jumps to an 11-bit address using the modifier as the high bits and the operand as the low bits
#define JMP 0
///Sets the accumulator to the sum of the accumulator and a value
#define ADD 1
///Sets the accumulator to the difference of the accumulator and a value
#define SUB 2
///Left shifts the accumulator by a value
#define LSH 3
///Right shifts the accumulator by a value
#define RSH 4
///Sets the accumulator to the bitwise AND of the accumulator and a value
#define AND 5
///Sets the accumulator to the bitwise OR of the accumulator and a value
#define OR  6
///Sets the accumulator to the bitwise XOR of the accumulator and a value
#define XOR 7
///Load Accumulator from Register
#define LDA 8
///Store Accumulator to Register
#define STA 9
///Conditional Jump
#define CND 10
///Push to Program Counter Stack
#define PSH 11
///Pop from Program Counter Stack
#define POP 12
///Compare Accumulator with Register to Condition Flag
#define CMP 13
///Immediate Comparison
#define ICP 14
///Value Stack
#define STK 15
///Read Port to Accumulator
#define RPA 16
///Read Port to Register
#define RPR 17
///Write Port from Accumulator
#define WPA 18
///Write Port from Register
#define WPR 19