#include <string.h>

struct String {
	unsigned int length;
	char *ptr;
};

struct StringList {
	struct String content;
	struct StringList next;
};


/// @brief Checks to see if this character is a quote character
/// @param x the character to check
/// @return true if x is a quote character, false otherwise
inline int Char_isQuote(char x) {
	return x == '"' || x == '\'';
}

/// @brief Creates a new String given a character array
struct String String_new(char *string);

/// @brief Checks to see if a String begins with a given prefix
/// @param self The string to check
/// @param prefix The string to compare to
/// @return Whether the string begins with prefix or not
int String_beginsWith(struct String self, struct String prefix);

/// @brief Checks to see if a character is in the string
/// @param self The string to check
/// @param x The character to check for
/// @return Whether the string self has the character x
int String_containsChar(struct String self, const x);

/// @brief Replaces x for y in self
/// @param self The string to be modified
/// @param x The character to be replaced
/// @param y The character to replace with
/// @return A modified copy of the original string with the characters replaced
struct String String_replaceChar(struct String self, const char x, const char y);

/// @brief Splits a string into a StringList
/// @param self String to split
/// @param x Character to split upon
/// @return A string list containing the left and right strings
struct StringList String_split(struct String self, const char x);

struct String String_joinStrings(struct String left, struct String right);

struct String String_removeChar(struct String self, const char x);

struct String String_toLower(struct String self);