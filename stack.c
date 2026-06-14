#include "stack.h"

void stack_init(Stack *s)
{
    s->top = -1;
}

int stack_empty(const Stack *s)
{
    return s->top == -1;
}

int stack_size(const Stack *s)
{
    return s->top + 1;
}

int stack_push(Stack *s, double value)
{
    if (s->top >= STACK_MAX_SIZE - 1)
        return 0;
    s->data[++(s->top)] = value;
    return 1;
}

int stack_pop(Stack *s, double *out)
{
    if (s->top < 0)
        return 0;
    *out = s->data[(s->top)--];
    return 1;
}

int stack_peek(const Stack *s, double *out)
{
    if (s->top < 0)
        return 0;
    *out = s->data[s->top];
    return 1;
}
