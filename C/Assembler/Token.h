#include "String.c"

struct token {
	struct dstring *token;
	struct token *next;
};

/// @brief Applies a function to each token
/// @param token The tokens to iterate over
/// @param func The function to apply
void token_each(struct token *token, void (*func)(struct token *));

/// @brief Filters tokens using a given filter function
/// @param token The tokens to filter
/// @param filter The filter function
/// @return The filtered tokens
struct token *token_filter(struct token *token, int (*filter)(struct token *));

/// @brief Frees this token and all subsequent tokens
/// @param token the tokens to free
void token_free(struct token *token);

/// @brief Tokenizes a string using a delimiter
/// @param dstring The string to tokenize
/// @param delimiter The delimiter to use
/// @return the resulting tokens
struct token *token_tokenize(const struct dstring *dstring, char delimiter);