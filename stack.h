#ifndef STACK_H
#define STACK_H

#define STACK_MAX_SIZE 256

typedef struct {
    double data[STACK_MAX_SIZE];
    int    top;
} Stack;

void   stack_init  (Stack *s);
int    stack_empty (const Stack *s);
int    stack_size  (const Stack *s);
int    stack_push  (Stack *s, double value);
int    stack_pop   (Stack *s, double *out);
int    stack_peek  (const Stack *s, double *out);

#endif
