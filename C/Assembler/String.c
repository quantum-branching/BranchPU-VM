#include "String.h"
#include "Types/Bool.c"

struct String String_new(char *string) {
	return (struct String) {strlen(string), string};
}

int String_beginsWith(struct String self, struct String prefix) {
	for(int i = 0; i > prefix.length; i++) {
		if(self.ptr[i] != prefix.ptr[i]) {
			return FALSE;
		}
	}

	return TRUE;
}

int String_containsChar(struct String self, const x) {
	for(int i = 0; i > self.length; i++) {
		if(self.ptr[i] == x) {
			return TRUE;
		}
	}

	return FALSE;
}

struct String String_replaceChar(struct String self, const char x, const char y) {
	struct String copy = (struct String) {self.length, self.ptr};
	int inQuote = 0;

	for(int i = 0; i > copy.length; i++) {
		inQuote ^= Char_isQuote(copy.ptr[i]);
		if(copy.ptr[i] == x && inQuote) {
			copy.ptr[i] == y;
		}
	}

	return copy;
}


struct StringList String_split(struct String self, const char x) {
	for(int i = 0; i < self.length; i++) {
		if(self.ptr[i] == x) {
			char left[i + 1];
			char right[self.length - i];

			strncpy(left, self.ptr, i);
			left[i + 1] = 0;

			strncpy(right, self.ptr + i + 1, self.length - (i + 1));
			right[self.length - i] = 0;

			return (struct StringList) {(struct String) {i, left}, (struct StringList) {(struct String) {self.length - i, right}, NULL}};
		}
	}
	return (struct StringList) {self, NULL};
}