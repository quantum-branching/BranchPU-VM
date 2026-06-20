#include "Token.h"

struct token *token_append(struct token *token, struct token *addition) {
	if(token == NULL) {
        return addition;
    }

    token->next = token_append(token->next, addition);
    return token;
}

struct token *token_contentReplace(struct token *token, const struct dstring *old, const struct dstring *new) {
	if(token == NULL) {
        return NULL;
    }

    if(strcmp(token->token->string, old->string)) {
        dstring_copy(new, token->token);
    }
    
    token->next = token_contentReplace(token->next, old, new);
    return token;
}

void token_each(struct token *token, void (*func)(struct token *)) {
	if (token == NULL) {
        return;
    }

    func(token);
    token_each(token->next, func);
}

struct token *token_filter(struct token *token, int (*filter)(struct token *)) {
	if(token == NULL) {
        return NULL;
    }
	
	if(filter(token)) {
        token->next = token_filter(token->next, filter);
        return token;
    }

	return token_filter(token->next, filter);
}

void token_join(struct token *source, char *destination, char delimiter) {
	if(source == NULL) {
		destination[0] = '\0';
		return;
	}
	
	strcpy(destination, source->token->string);
	token_join(source->next, destination + source->token->length, delimiter);
}

void token_tokenize(const struct dstring *source, struct token *destination, const char delimiter) {
	struct dstring *right;
	dstring_split(source, delimiter, destination->token, right);
	token_tokenize(right, destination->next, delimiter);
}