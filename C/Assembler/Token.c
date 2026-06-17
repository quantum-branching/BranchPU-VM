#include "Token.h"

void token_each(struct token *token, void (*func)(struct token *)) {
    if (token == NULL) {
        return;
    }

    func(token);
    token_each(token->next, func);
}

struct token *token_filter(struct token *token, int (*filter)(struct token *)) {
    if(filter(token)) {
        token->next = token_filter(token->next, filter);
        return token;
    } else {
        return token_filter(token->next, filter);
    }
}

void token_free(struct token *token) {
    if (token == NULL) {
        return;
    }
    token_free(token->next);
    free(token);
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