#include "Stack.h"

inline void stack_push(struct Stack *stack, const int value) {
    if (stack->index < STACK_MASK) {
        stack->data[stack->index++] = value;
    }
}

inline int stack_pop(struct Stack *stack) {
    if (stack->index > 0) {
        return stack->data[--stack->index];
    } else {
        return 0;
    }
}

inline int stack_peek(const struct Stack *stack, const int offset) {
    if (offset >= 0 && offset < stack->index) {
        return stack->data[stack->index - 1 - offset];
    } else {
        return 0;
    }
}