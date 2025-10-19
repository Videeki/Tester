#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "stringUtils.h"

typedef struct sequences Sequences;
typedef struct keys Keys;
typedef struct params Parameters;

struct sequences
{
    char* sequence;
    uint8_t copy;
    Sequences* next;
};

struct keys
{
    char* sequence;
    char* key;
    uint8_t copy;
    Keys* next;
};

struct params
{
    char* name;
    char* value;
    Parameters* next;
};

int parser(Sequences* seqs, Keys* keys, Parameters* params, const char* path);

Sequences* sequence_append(Sequences* list, const char* sequence, uint8_t copy);
int sequence_index(Sequences* list, const char* sequence);
void sequence_free(Sequences* list);

Keys* keys_append(Keys* list, char* sequence, const char* key, uint8_t copy);
void keys_free(Keys* list);

Parameters* param_append(Parameters* list, const char* param);
char* get_parameter(Parameters* list, const char* parameter);
void param_free(Parameters* list);

#endif  /* PARSER_H */