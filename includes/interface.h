#ifndef INTERFACE_H
#define INTERFACE_H


#include <stdint.h>

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

typedef struct tester
{
    Sequences* seqs;
    Keys* keys;
    Parameters* params;
}TESTER;


#endif /* INTERFACE_H */