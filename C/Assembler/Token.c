#include "Token.h"

struct token *token_contentReplace(struct token *token, const struct dstring *old, const struct dstring *new) {
    if(token == NULL) {
        return NULL;
    }

    if(strcmp(token->token->string, old->string)) {
        dstring_free(token->token);
        token->token = dstring_copy(new);
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

	struct token *next = token_filter(token->next, filter);
	free(token);
	return next;
}

void token_free(struct token *token) {
    if (token == NULL) {
        return;
    }
    token_free(token->next);
    free(token);
}

struct dstring *token_join(struct token *token, char delimiter) {
	struct token *current = token;
	struct dstring result;
	char *contents;
	int length = 0;

	while(current != NULL) {
		length += current->token->length + 1;
		current = current->next;
	}

	current = token;
	contents = malloc(length + 1);
	contents[0] = '\0';
	while(current != NULL) {
		strcat(contents, current->token->string);
		current = current->next;
	}

	return dstring_new(contents);
}

struct token *token_tokenize(const struct dstring *dstring, char delimiter) {
    if (dstring->length == 0) {
        return NULL;
    }
    
    struct token *token = malloc(sizeof(struct token));

    struct dstring *left = DSTRING_EMPTY;
    struct dstring *right = DSTRING_EMPTY;

    dstring_split(dstring, delimiter, left, right);

    token->token = left;
    token->next = token_tokenize(right, delimiter);

    return token;
}

struct dstring *token_stringify(struct token *token) {
    return NULL;
}