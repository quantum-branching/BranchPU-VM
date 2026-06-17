#include "String.h"

struct dstring *dstring_copy(struct dstring *dstring) {
	return dstring_new(dstring->string);
}

void dstring_free(struct dstring *dstring) {
	free(dstring->string);
	free(dstring);
}

struct dstring *dstring_new(const char *string) {
	struct dstring *ptr = malloc(sizeof(struct dstring));
	char *contents = malloc(strlen(string) + 1);

	strcpy(contents, string);

	ptr->string = contents;
	ptr->length = strlen(string);

	return ptr;
}

struct dstring *dstring_remove(struct dstring *dstring, const char c) {
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

	return dstring;
}

struct dstring *dstring_replace(struct dstring *dstring, const char x, const char y) {
	for(int i = 0; i < dstring->length; i++) {
		if(dstring->string[i] == x) {
			dstring->string[i] = y;
		}
	}

	return dstring;
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

	left->string = realloc(left->string, length + 1);
	strncpy(left->string, string, length);
	left->string[length] = '\0';
	left->length = length;

	right->string = realloc(right->string, 1);
	right->string[0] = '\0';
	right->length = 0;
}