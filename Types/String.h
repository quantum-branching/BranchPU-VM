#include <string.h>
#include <stdlib.h>

#define debug //printf

#define ERROR -1
#define TRUE 1
#define FALSE 0

#define isSpace(x) (((x) <= ' ') && ((x) != '\0'))
#define isNumber(x) ((x) >= '0' && (x) <= '9')
#define isLetter(x) (((x) >= 'A' && (x) <= 'Z') || ((x) >= 'a' && (x) <= 'z') || ((x) == '_'))
#define String_hasWord(x) (*String_nextWord(x))

#define Substring_new(string, length) ((struct Substring) {string, length})
#define TokenList_new(type, token, next) ((struct TokenList) {type, token, next})

#define TOKEN_ERROR 0
#define TOKEN_ID 1
#define TOKEN_VALUE 2
#define TOKEN_SYMBOL 3

struct Substring {
	char *start;
	int length;
};

struct TokenList {
	int type;
	struct Substring token;
	struct TokenList *next;
};

int String_toInt(const char *str);

/// @brief Finds whether the next token produced in the string will be an identifier
/// @param string The string to check
/// @return Whether or not the next token in the string will be an identifier
int String_isIdentifier(const struct Substring string);

/// @brief Finds whether the next token produced in the string will be a symbol
/// @param string The string to check
int String_isSymbol(const struct Substring string);

/// @brief Finds whether the next token produced in the string will be a value
/// @param string The string to check
/// @return Length of token if value, 0 otherwise
/// @exception Returns -1 if character is defined with a length greater than 1
int String_isValue(const struct Substring string);

/// @brief Finds the next token in the string
/// @param string The string to search for the next token
/// @param tokens Tokens to append to
/// @return A new substring that is after the next token
struct Substring String_nextToken(const struct Substring string, struct TokenList *tokens);

/// @brief Finds the next character that proceeds a space
/// @param str The string to find the next word in
/// @return A string that immediately comes after the next space
/// @exception If there is no spaces in the string returns a null string
char *String_nextWord(char *str);

/// @brief Appends a token to a TokenList
/// @param list The list to append the token to
/// @param type The type of the token to append
/// @param token The substring of the token to append
/// @return A TokenList with given token appended
/// @note This function allocated memory, to free it please use Token_free() on the first token
/// appended by this function
struct TokenList *Token_append(struct TokenList *list, const int type, struct Substring token);

/// @brief Frees a list of tokens
/// @param tokens The tokens to free
void Token_free(struct TokenList *tokens);