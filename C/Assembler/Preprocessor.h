#include "Token.c"

struct dstring *substitute;
struct dstring *definition;

void define(struct token *line);

void include(struct token *token);

int isBlank(struct token *line);

int enumDirective(const struct token *token);

struct token *parseDirective(struct token *token, struct token *current);