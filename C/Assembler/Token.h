#include "String.c"

/// @brief A token that contains a dynamic string and optionally another token
/// @param token The dynamic string
/// @param next The next token in a linked list of tokens
struct token {
	struct dstring *token;
	struct token *next;
};

/// @brief Appends a new token to the end of a list of tokens
/// @param token The list of tokens to append to
/// @param addition The new token to append
/// @return The updated list of tokens
struct token *token_append(struct token *token, struct token *addition);

/// @brief Replaces content in a tokens
/// @param token Tokens to process
/// @param old The old content to replace
/// @param new The new content to replace with
/// @return Processed tokens with content replaced
struct token *token_contentReplace(struct token *token, const struct dstring *old, const struct dstring *new);

/// @brief Applies a function to each token
/// @param token The tokens to iterate over
/// @param func The function to apply
void token_each(struct token *token, void (*func)(struct token *));

/// @brief Filters tokens using a given filter function
/// @param token The tokens to filter
/// @param filter The filter function
/// @return The filtered tokens
struct token *token_filter(struct token *token, int (*filter)(struct token *));

/// @brief Joins tokens with a delimiter to create a dynamic string
/// @param token The tokens to join
/// @param destination The dynamic string to store the result
/// @param delimiter The delimiter to use
void token_join(struct token *source, char *destination, char delimiter);

/// @brief Tokenizes a dynamic string into a list of tokens
/// @param source dynamic string to tokenize
/// @param destination location to store the tokens
/// @param delimiter character to separate tokens
void token_tokenize(const struct dstring *source, struct token *destination, const char delimiter);