#include "Token.c"

struct dstring *substitute;
struct dstring *definition;

struct token *define(struct token *token);

void include(struct token *token);

int isBlank(struct token *line);

int enumDirective(const struct token *token);

struct token *parseDirective(struct token *token, struct token *current);