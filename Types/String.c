#include "String.h"

/// @brief Converts a string to an integer
/// @param str The string to convert
/// @return The integer contained in the string
/// @exception Returns -1 if no integer exists
int String_toInt(const char *str) {
	int result = 0;
	int NaN = 1;
	
	while(*str != '\0' || *str != ' ') {
		if(isNumber(*str)) {
			result = 10 * result + *str - '0';
			NaN = 0;
		}
	}

	return result - NaN;
}

int String_isIdentifier(const struct Substring string) {
	char x = string.start[0];
	int length = 0;
	
	if(isNumber(x)) {
		return 0;
	}
	
	while(isNumber(x) || isLetter(x)) {
		x = string.start[++length];
	}

	if(length > string.length) {
		return string.length;
	}

	return length;
}

int String_isSymbol(const struct Substring string) {
	int length = 0;
	
	if(*string.start == '{' || *string.start == '}' || *string.start == '(' || *string.start == ')' || *string.start == ';') {
		return TRUE;
	}

	if(length > string.length) {
		return string.length;
	}

	return length;
}

int String_isValue(const struct Substring string) {
	int length = 0;

	if(*string.start == '\'') {
		if(string.start[++length] == '\\') {
			length++;
		}
		if(string.start[++length] != '\'') {
			return ERROR;
		}
	}

	if(*string.start == '"') {
		length++;
		while(string.start[length] != '"')  {
			length++;
		}
		length++;
	}

	while(isNumber(string.start[length])) {
		length++;
	}
	
	if(length > string.length) {
		return string.length;
	}

	return length;
}

struct Substring String_nextToken(const struct Substring string, struct TokenList *tokens) {
	int length = 0;
	char *ptr = string.start;
	int stringLength = string.length;
	struct Substring copy;

	// Ignores whitespace
	while(isSpace(*ptr)) {
		ptr++;
		stringLength--;
	}

	// Handles empty substrings
	if(!*ptr || stringLength <= 0) {
		return Substring_new("", 0);
	}

	copy = Substring_new(ptr, stringLength);

	// Handles symbols
	length = String_isSymbol(copy);
	if(length) {
		tokens = Token_append(tokens, TOKEN_SYMBOL, Substring_new(ptr, length));
		return Substring_new(ptr + length, stringLength - length);
	}

	// Handles identifiers
	length = String_isIdentifier(copy);
	if(length) {
		tokens = Token_append(tokens, TOKEN_ID, Substring_new(ptr, length));
		return Substring_new(ptr + length, stringLength - length);
	}

	// Handles values
	length = String_isValue(copy);
	if(length) {
		tokens = Token_append(tokens, TOKEN_VALUE, Substring_new(ptr, length));
		return Substring_new(ptr + length, stringLength - length);
	}

	debug("Error: Unhandled Input\n");
	return Substring_new(ptr, stringLength);
}

char *String_nextWord(char *str) {
	char *copy = str;
	while(*copy != ' ') {
		if(!*copy) {
			return copy;
		}

		copy++;
	}

	return ++copy;
}

struct TokenList *Token_append(struct TokenList *list, const int type, struct Substring token) {
	if(!list) {
		struct TokenList *result = malloc(sizeof(struct TokenList));
		result->type = type;
		result->token = token;
		result->next = NULL;
		return result;
	}

	list->next = Token_append(list->next, type, token);
	return list;
}

void Token_free(struct TokenList *tokens) {
	if(tokens) {
		Token_free(tokens->next);
		free(tokens);
	}
}

