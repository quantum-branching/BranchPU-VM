#include "Stack.h"

void push(struct Stack *stack, const int value) {
    if (stack->index < STACK_MASK) {
        stack->data[stack->index++] = value;
    }
}

int pop(struct Stack *stack) {
    if (stack->index > 0) {
        return stack->data[--stack->index];
    } else {
        return 0;
    }
}

int peek(const struct Stack *stack, const int offset) {
    if (offset >= 0 && offset < stack->index) {
        return stack->data[stack->index - 1 - offset];
    } else {
        return 0;
    }
}