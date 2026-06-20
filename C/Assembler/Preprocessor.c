#include "Preprocessor.h"

#define DIR_REMOVEBLANK 1
#define DIR_DEFINE 2
#define DIR_INCLUDE 3

struct dstring *substitute;
struct dstring *definition;

void define(struct token *line) {
	struct token *tokens;
	token_tokenize(line->token, tokens, ' ');
	token_contentReplace(tokens, substitute, definition);

	token_join(tokens, line->token->string, ' ');
	line->token->length = strlen(line->token->string);
}

int isBlank(struct token *line) {
	return line->token->length;
}

int enumDirective(const struct token *token) {
	if(strcmp(token->token->string, "#removeblank") == 0) {
		return DIR_REMOVEBLANK;
	}

	if(strcmp(token->token->string, "#define") == 0) {
		return DIR_DEFINE;
	}

	if(strcmp(token->token->string, "#include") == 0 || strcmp(token->token->string, "import") == 0) {
		return DIR_INCLUDE;
	}

	return 0;
}

struct token *parseDirective(struct token *program, struct token *current) {
	if(current == NULL) {
		return program;
    }

	struct token *line;
	token_tokenize(current->token, line, ' ');

	switch(enumDirective(line)) {
		case DIR_REMOVEBLANK:
			debug0("Removing blank lines\n");
			program = token_filter(program, isBlank);
		case DIR_DEFINE:
			if(line->next == NULL) {
				printf("#define directive expects 2 arguments and recieved 0\n");
			} else if(line->next->next == NULL) {
				printf("Can not define %s, #define expects 2 arguments and recieved 1\n", line->next->token->string);
			} else {
				debug2("Defining %s as %s\n", line->next->token->string, line->next->next->token->string);
				substitute = line->next->token;
				definition = line->next->next->token;
				token_each(program, define);
			}
		case DIR_INCLUDE:
			struct token *include; //TODO: Find the include file
            program = token_append(program, include);
	}

	return parseDirective(program, current->next);
}