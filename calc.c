#include "calc.h"
#include "stack.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E  2.71828182845904523536
#endif

typedef enum {
    TOKEN_NUMBER,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LN,
    TOKEN_LOG,
    TOKEN_PI,
    TOKEN_E,
    TOKEN_INVALID
} TokenType;

static int is_integer_string(const char *s)
{
    if (*s == '\0') return 0;
    if (*s == '-') s++;
    if (*s == '\0') return 0;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

static TokenType classify(const char *raw, double *number_out)
{
    if (strcmp(raw, "+")   == 0) return TOKEN_ADD;
    if (strcmp(raw, "-")   == 0) return TOKEN_SUB;
    if (strcmp(raw, "*")   == 0) return TOKEN_MUL;
    if (strcmp(raw, "/")   == 0) return TOKEN_DIV;
    if (strcmp(raw, "ln")  == 0) return TOKEN_LN;
    if (strcmp(raw, "log") == 0) return TOKEN_LOG;
    if (strcmp(raw, "pi")  == 0) return TOKEN_PI;
    if (strcmp(raw, "e")   == 0) return TOKEN_E;

    if (is_integer_string(raw)) {
        if (number_out) *number_out = (double)atol(raw);
        return TOKEN_NUMBER;
    }

    return TOKEN_INVALID;
}

static int arity(TokenType t)
{
    switch (t) {
        case TOKEN_NUMBER: return 0;
        case TOKEN_ADD: return 2;
        case TOKEN_SUB: return 2;
        case TOKEN_MUL: return 2;
        case TOKEN_DIV: return 2;
        case TOKEN_LN:  return 1;
        case TOKEN_LOG: return 2;
        case TOKEN_PI:  return 0;
        case TOKEN_E:   return 0;
        default:        return -1;
    }
}

int validate(char tokens[][MAX_TOKEN_LENGTH], int count,
             int *error_token, char *error_msg, size_t msg_size)
{
    int depth = 0;

    for (int i = 0; i < count; i++) {
        double dummy = 0.0;
        TokenType t = classify(tokens[i], &dummy);

        if (t == TOKEN_INVALID) {
            if (error_token) *error_token = i;
            snprintf(error_msg, msg_size,
                     "Error: token %d (\"%s\") is not a valid number or operation.",
                     i + 1, tokens[i]);
            return 0;
        }

        int a = arity(t);

        if (a == 0) {
            depth++;
        } else {
            if (depth < a) {
                if (error_token) *error_token = i;
                snprintf(error_msg, msg_size,
                         "Error: token %d (\"%s\") requires %d operand(s) but the stack only has %d.",
                         i + 1, tokens[i], a, depth);
                return 0;
            }
            depth -= a;
            depth++;
        }
    }

    if (depth != 1) {
        if (error_token) *error_token = count - 1;
        snprintf(error_msg, msg_size,
                 "Error: %d value(s) left on the stack after evaluation (expected exactly 1).",
                 depth);
        return 0;
    }

    return 1;
}

int evaluate(char tokens[][MAX_TOKEN_LENGTH], int count,
             double *result, char *error_msg, size_t msg_size)
{
    Stack s;
    stack_init(&s);

    for (int i = 0; i < count; i++) {
        double number = 0.0;
        TokenType t = classify(tokens[i], &number);
        double a, b, res;

        switch (t) {
            case TOKEN_NUMBER:
                stack_push(&s, number);
                break;

            case TOKEN_PI:
                stack_push(&s, M_PI);
                break;

            case TOKEN_E:
                stack_push(&s, M_E);
                break;

            case TOKEN_ADD:
                if (!stack_pop(&s, &b) || !stack_pop(&s, &a)) {
                    snprintf(error_msg, msg_size, "Error: insufficient operands for '+' at token %d.", i + 1);
                    return 0;
                }
                stack_push(&s, a + b);
                break;

            case TOKEN_SUB:
                if (!stack_pop(&s, &b) || !stack_pop(&s, &a)) {
                    snprintf(error_msg, msg_size, "Error: insufficient operands for '-' at token %d.", i + 1);
                    return 0;
                }
                stack_push(&s, a - b);
                break;

            case TOKEN_MUL:
                if (!stack_pop(&s, &b) || !stack_pop(&s, &a)) {
                    snprintf(error_msg, msg_size, "Error: insufficient operands for '*' at token %d.", i + 1);
                    return 0;
                }
                stack_push(&s, a * b);
                break;

            case TOKEN_DIV:
                if (!stack_pop(&s, &b) || !stack_pop(&s, &a)) {
                    snprintf(error_msg, msg_size, "Error: insufficient operands for '/' at token %d.", i + 1);
                    return 0;
                }
                if (b == 0.0) {
                    snprintf(error_msg, msg_size,
                             "Error: division by zero at token %d (\"/\").", i + 1);
                    return 0;
                }
                stack_push(&s, a / b);
                break;

            case TOKEN_LN:
                if (!stack_pop(&s, &a)) {
                    snprintf(error_msg, msg_size, "Error: insufficient operands for 'ln' at token %d.", i + 1);
                    return 0;
                }
                if (a <= 0.0) {
                    snprintf(error_msg, msg_size,
                             "Error: ln is undefined for non-positive values "
                             "(token %d, argument = %g).", i + 1, a);
                    return 0;
                }
                stack_push(&s, log(a));
                break;

            case TOKEN_LOG:
                if (!stack_pop(&s, &a) || !stack_pop(&s, &b)) {
                    snprintf(error_msg, msg_size, "Error: insufficient operands for 'log' at token %d.", i + 1);
                    return 0;
                }
                if (b <= 0.0 || b == 1.0) {
                    snprintf(error_msg, msg_size,
                             "Error: invalid logarithm base (%g) at token %d.", b, i + 1);
                    return 0;
                }
                if (a <= 0.0) {
                    snprintf(error_msg, msg_size,
                             "Error: log is undefined for non-positive values "
                             "(argument = %g) at token %d.", a, i + 1);
                    return 0;
                }
                res = log(a) / log(b);
                stack_push(&s, res);
                break;

            default:
                snprintf(error_msg, msg_size,
                         "Internal error: unknown token type at position %d.", i + 1);
                return 0;
        }
    }

    return stack_pop(&s, result);
}
