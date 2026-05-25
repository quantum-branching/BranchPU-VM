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
void push(struct Stack *stack, const int value) {
    if (stack->index < STACK_LIMIT) {
        stack->data[stack->index++] = value;
    }
}

/// Pops the value off the stack and returns it
/// @param stack The stack to pop the value from
/// @return The last value pushed onto the stack, or 0 if the 
/// stack is empty
int pop(struct Stack *stack) {
    if (stack->index > 0) {
        return stack->data[--stack->index];
    } else {
        return 0;
    }
}

/// Peeks at the value at a certain offset from the top of the 
///stack without modifying the stack
/// @param stack The stack to peek the value from
/// @param offset The offset from the top of the stack to peek 
/// at, where 0 is the top of the stack
/// @return The value at the specified offset from the top of 
/// the stack, or 0 if the offset is out of bounds
int peek(const struct Stack *stack, const int offset) {
    if (offset >= 0 && offset < stack->index) {
        return stack->data[stack->index - 1 - offset];
    } else {
        return 0;
    }
}