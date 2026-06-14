#include <stdio.h>
#include "tokenizer.h"

#define MAX_EXPRESSION_LENGTH 500

int main()
{
    char expression[MAX_EXPRESSION_LENGTH];
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];

    printf("Enter postfix expression:\n");
    fgets(expression, sizeof(expression), stdin);

    int tokenCount = tokenize(expression, tokens);

    printf("\nTokens:\n");

    for (int i = 0; i < tokenCount; i++)
    {
        printf("%d: %s\n", i + 1, tokens[i]);
    }

    return 0;
}