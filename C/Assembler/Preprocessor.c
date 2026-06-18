#include "Arena.c"
#include "Preprocessor.h"

#define DIR_REMOVEBLANK 1
#define DIR_DEFINE 2
#define DIR_INCLUDE 3

struct dstring *defined;
struct dstring *definition;

void define(struct token *line) {
	struct token *tokens = token_tokenize(line->token, ' ');
}

void include(struct token *token) {
	
}

/// @brief Checks to see if a line is blank
/// @param line Line to check if blank
/// @return True if the line is blank, false otherwise
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
    
	enterArena(PAGE_SIZE);
    struct token *line = token_tokenize(current->token, ' ');

    switch(enumDirective(line)) {
        case DIR_REMOVEBLANK:
			// printf("Removing blank lines\n");
            program = token_filter(program, isBlank);
        case DIR_DEFINE:
            if(line->next == NULL) {
				printf("#define directive expects 2 arguments and recieved 0\n");
			} else if(line->next->next == NULL) {
				printf("Can not define %s, #define expects 2 arguments and recieved 1\n", line->next->token->string);
			} else {
				// printf("Defining %s as %s\n", line->next->token->string, line->next->next->token->string);
				defined = line->next->token;
				definition = line->next->next->token;
				token_each(program, define);
			}
        case DIR_INCLUDE:
            break;
    }
    
	exitArena();
    return parseDirective(program, current->next);
}