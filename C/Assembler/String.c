///@brief String.c but requires the caller pass available memory

#include "String.h"

void dstring_copy(const struct dstring *source, struct dstring *destination) {
	destination->length = source->length;
	destination->string = source->string;
}

struct dstring dstring(char *string) {
	return (struct dstring) {string, strlen(string)};
}

void dstring_join(const struct dstring *first, const struct dstring *second, struct dstring *result) {
	strncpy(result->string, first->string, first->length);
	strncat(result->string, second->string, second->length);
	result->length = first->length + second->length;
	result->string[result->length] = '\0';
}

void dstring_remove(struct dstring *dstring, const char c) {
	int inString = 0;

	for(int i = 0; i < dstring->length; i++) {
		if(dstring->string[i] == '"') {
			inString = !inString;
		}

		if(dstring->string[i] == c && !inString) {
			memmove(dstring->string + i, dstring->string + i + 1, dstring->length - i);
			dstring->length--;
			i--;
		}
	}
}

void dstring_replace(struct dstring *dstring, const char x, const char y) {
	int inString = 0;
	
	for(int i = 0; i < dstring->length; i++) {
		if(dstring->string[i] == '"') {
			inString = !inString;
		}

		if(dstring->string[i] == x && !inString) {
			dstring->string[i] = y;
		}
	}
}

int dstring_sizeof(const struct dstring *dstring) {
	return sizeof(struct dstring) + sizeof(char) * (dstring->length + 1);
}

void dstring_split(const struct dstring *dstring, const char delimiter, struct dstring *left, struct dstring *right) {
	int length = dstring->length;
	char *string = dstring->string;

	int inString = 0;

	for(int i = 0; i < length; i++) {
		if(string[i] == '"') {
			inString = !inString;
		}

		if(string[i] == delimiter && !inString) {
			left->string = realloc(left->string, i + 1);
			strncpy(left->string, string, i);
			left->string[i] = '\0';
			left->length = i;

			right->string = realloc(right->string, length - i);
			strncpy(right->string, string + i + 1, length - i - 1);
			right->string[length - i - 1] = '\0';
			right->length = length - i - 1;

			return;
		}
	}
}

void dstring_tolower(struct dstring *dstring) {
	for(int i = 0; i < dstring->length; i++) {
		dstring->string[i] = tolower(dstring->string[i]);
	}
}