#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int string_trim_before(const char* str, int offset);
int string_trim_after(const char* str, int offset);
int index_char(char* str, char chr);

typedef struct strlist stringList;

struct strlist
{
    char* str;
    int strLen;
    stringList* next;
};

stringList* stringList_from_string(stringList* list, char* str, char* separator);
char* stringList_select_string(stringList* list, int index);
void stringList_clear(stringList* list);



#endif  /* STRINGUTILS */