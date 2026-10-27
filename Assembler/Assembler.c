#include "Assembler.h"

int collapse(char x, char y, char z) {
	return (tolower(x) - '`') * 26 *26 + (tolower(y) - '`') * 26 + (tolower(z) - '`');
}

char asm2op(char *mnuemonic) {
	switch(collapse(mnuemonic[0], mnuemonic[1], mnuemonic[2])) {
		case STRING_JMP: return JMP;
		case STRING_ADD: return ADD;
		case STRING_SUB: return SUB;
		case STRING_LSH: return LSH;
		case STRING_RSH: return RSH;
		case STRING_AND: return AND;
		case STRING_OR:  return OR; 
		case STRING_XOR: return XOR;
		case STRING_LDA: return LDA;
		case STRING_LDI: return LDI;
		case STRING_STA: return STA;
		case STRING_CND: return CND;
		case STRING_PSH: return PSH;
		case STRING_POP: return POP;
		case STRING_CMP: return CMP;
		case STRING_ICP: return ICP;
		case STRING_STK: return STK;
		case STRING_RPA: return RPA;
		case STRING_RPR: return RPR;
		case STRING_WPA: return WPA;
		case STRING_WPR: return WPR;
		case STRING_PEK: return PEK;
		default:
			char copy[4];
			strncpy(copy, mnuemonic, 3);
			copy[3] = 0;
			printf("Unknown Mnuemonic: %s\n", copy);
			return -1;
	}
}

char asm2flag(const char opcode, char *operands) {
	int immediate = operands[0];
	int address3 = String_toInt(operands);

	if(!String_hasWord(operands)) {
		address3 >>= 8;
	}

	address3 &= 8;

	switch (opcode) {
		case JMP:
			return address3;
		case ADD:
			return immediate;
		case SUB:
			return immediate;
		case LSH:
			return immediate;
		case RSH:
			return immediate;
		case AND:
			return immediate;
		case OR:
			return immediate;
		case XOR:
			return immediate;
		case LDA:
			return 0;
		case LDI:
			return 1;
		case STA:
			return 0;
		case CND:
			return address3;
		case PSH:
			return address3;
		case POP:
			return 0;
		case CMP:
			return address3;
		case ICP:
			return address3;
		case STK:
			return address3;
		case RPA:
			return address3;
		case RPR:
			return address3;
		case WPA:
			return address3;
		case WPR:
			return address3;
		case PEK:
			return immediate;
		default:
			printf("Unknown Opcode: %d", opcode);
			return -1;
	}
}

char asm2args(char *operands) {
	if(!String_hasWord(operands)) {
		return String_toInt(operands) & 255;
	}

	return String_toInt(String_nextWord(operands)) & 255;
}

void removeComments(const char *source, char *destination) {

}

struct Assembly Assembly_fromString(char *line, char *label, const int lineNo) {
	char copy[strlen(line) + 1];
	char operand = 0;
	char upper = 0;
	char lower = 0;

	removeComments(line, copy);
	operand = asm2op(copy);
	upper = operand << 3 | asm2flag(operand, String_nextWord(copy));
	lower = asm2args(String_nextWord(copy)); // Add Lower Byte

	return Assembly_new(label, lineNo, upper, lower);
}

void Definition_append(struct Definition *definitions, char *word, char *definition) {
	struct Definition *current = definitions;
	struct Definition *next = malloc(sizeof(struct Definition));

	while(current->next) {
		current = current->next;
	}

	next->word = word;
	next->definition = definition;

	current->next = next;
	return definitions;
}

void Defintition_free(struct Definition *definition) {
	if(definition) {
		Defintition_free(definition->next);
		free(definition);
	}
}