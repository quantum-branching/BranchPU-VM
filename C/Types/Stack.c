struct Stack

#define STACK_SIZE 256
#define STACK_LIMIT (STACK_SIZE - 1)

{
    int data[STACK_SIZE];
    int index;

};

/// Pushes a value onto the stack
/// @param stack The stack to push the value onto
/// @param value The value to push onto the stack
void push(struct Stack *stack, int value) {
    if (stack->index < STACK_LIMIT) {
        stack->data[stack->index++] = value;
    }
}

/// Pops the value off the stack and returns it
/// @param stack The stack to pop the value from
/// @return The last value pushed onto the stack, or 0 if the stack is empty
int pop(struct Stack *stack) {
    if (stack->index > 0) {
        return stack->data[--stack->index];
    } else {
        return 0;
    }
}