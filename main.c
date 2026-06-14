#include <stdio.h>
#include "tokenizer.h"
#include "calc.h"

#define MAX_EXPRESSION_LENGTH 500

int main(void)
{
    char expression[MAX_EXPRESSION_LENGTH];
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];

    printf("Enter postfix expression:\n");
    fgets(expression, sizeof(expression), stdin);

    int count = tokenize(expression, tokens);

    if (count == 0) {
        fprintf(stderr, "Error: empty expression.\n");
        return 1;
    }

    int  error_token = -1;
    char error_msg[256];

    if (!validate(tokens, count, &error_token, error_msg, sizeof(error_msg))) {
        fprintf(stderr, "%s\n", error_msg);
        return 1;
    }

    double result = 0.0;

    if (!evaluate(tokens, count, &result, error_msg, sizeof(error_msg))) {
        fprintf(stderr, "%s\n", error_msg);
        return 1;
    }

    printf("Result: %g\n", result);
    return 0;
}
