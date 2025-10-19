#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int string_trim_before(const char* str, int offset);
int string_trim_after(const char* str, int offset);
int index_char(char* str, char chr);

#endif  /* STRINGUTILS */