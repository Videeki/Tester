#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>



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



#define PATH char*

PATH string_path_init(const char* path);

PATH string_path_append(PATH self, const char* pathItem);

/************************************************************/
/*  Use the NULL keyword to finish list of the parameters   */
/************************************************************/
PATH string_path_append_items(PATH self, ...);

void string_path_free(PATH self);



#define XPATH char*

XPATH string_xpath_init(const char* xpath);

XPATH string_xpath_append(XPATH self, const char* xpathItem);

/************************************************************/
/*  Use the NULL keyword to finish list of the parameters   */
/************************************************************/
XPATH string_xpath_append_items(XPATH self, ...);

void string_xpath_free(XPATH self);


#endif  /* STRINGUTILS */