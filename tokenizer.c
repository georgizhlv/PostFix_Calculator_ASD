#include <string.h>
#include "tokenizer.h"

int tokenize(char *expression, char tokens[][MAX_TOKEN_LENGTH])
{
    int count = 0;

    char *token = strtok(expression, " \t\n");

    while (token != NULL)
    {
        strcpy(tokens[count], token);
        count++;

        token = strtok(NULL, " \t\n");
    }

    return count;
}