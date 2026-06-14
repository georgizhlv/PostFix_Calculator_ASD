#ifndef CALC_H
#define CALC_H

#include <stddef.h>
#include "tokenizer.h"

int validate (char tokens[][MAX_TOKEN_LENGTH], int count,
              int *error_token, char *error_msg, size_t msg_size);

int evaluate (char tokens[][MAX_TOKEN_LENGTH], int count,
              double *result, char *error_msg, size_t msg_size);

#endif
