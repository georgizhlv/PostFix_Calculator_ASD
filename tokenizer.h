#ifndef TOKENIZER_H
#define TOKENIZER_H

#define MAX_TOKENS 100
#define MAX_TOKEN_LENGTH 20

int tokenize(char *expression, char tokens[][MAX_TOKEN_LENGTH]);

#endif