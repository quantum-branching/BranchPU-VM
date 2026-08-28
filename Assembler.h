#include "Types/String.c"
#include "Data/ISA.c"
#include <stdio.h>
#include <ctype.h>

#define Assembly_new(label, line, upper, lower) ((struct Assembly) {label, line, upper, lower})

struct Assembly {
    char *label;
    int line;
    char upper;
    char lower;
};

struct Definition {
    char *word;
    char *definition;
    struct Definition *next;
};

/// @brief Collapses three characters into a single integer
/// @param x First character
/// @param y Second character
/// @param z Third character
/// @return An integer
int collapse(char x, char y, char z);

/// @brief Converts a mnuemonic to an opcode
/// @param mnuemonic The mnuemonic to convert to an opcode
/// @return The opcode related to the mnuemonic
char asm2op(char *mnuemonic);

/// @brief Converts the operands into a flag
/// @param opcode The opcode to use to interpret the operands
/// @param operands The operands to convert into a flag
/// @return The flag related to the opcode and operands
char asm2flag(const char opcode, char *operands);

/// @brief Strips a comment from a line
/// @param source Line to strip comment from
/// @param destination Where to save the stripped line
void removeComments(const char *source, char *destination);

/// @brief Creates an assembly from string
/// @param line Line to convert
/// @param lineNo Linumber
/// @return Converted assembly
struct Assembly Assembly_fromString(char *line, char *label, const int lineNo);

/// @brief Appends a definintion at the end of a definition list
/// @param definitions A list of definitions
/// @param word A word to define
/// @param definition The definition for the word
void Definition_append(struct Definition *definitions, char *word, char *definition);

/// @brief Frees a definition
/// @param definition Definition to free
void Defintition_free(struct Definition *definition);