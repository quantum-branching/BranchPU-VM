#include "Token.c"

struct dstring *substitute;
struct dstring *definition;

void define(struct token *line);

/// @brief Checks to see if a line is blank
/// @param line Line to check if blank
/// @return True if the line is blank, false otherwise
int isBlank(struct token *line);

/// @brief Converts a token to a directive enum if it is a valid directive
/// @param token token to convert
/// @return enum if valid token, 0 otherwise
int enumDirective(const struct token *token);

/// @brief Parses the directives to be applied to the given program
/// @param program Program to apply directives to
/// @param current Current token in the program
/// @return The updated program with directives applied
struct token *parseDirective(struct token *program, struct token *current);