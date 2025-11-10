#include "stringUtils.h"


int string_trim_before(const char* str, int offset)
{
    int i = 0;
    for(i += offset; str[i] != '\0'; i++)
        if(str[i] > 32) break;
    
    return i;
}


int string_trim_after(const char* str, int offset)
{
    int i = 0;
    for(i = strlen(str) - offset; i != 0; i--)
    {
        if(str[i] > 32) break;
    }
    
    return i;
}

int index_char(char* str, char chr)
{
    for(int i = 0; str[i] != '\0'; i++)
        if(str[i] == chr)
            return i;

    return -1;
}

stringList* stringList_from_string(stringList* list, char* str, char* separator)
{
    char* token = strtok(str, separator);

    while(token != NULL)
    {   
        stringList* new = (stringList*)malloc(sizeof(stringList));
        new->next = NULL;
        new->strLen = strlen(token);

        //Trim the token
        int tokenBegin = string_trim_before(token, 0);
        int tokenEnd = string_trim_after(token, new->strLen + 1);
        int tokenLen = tokenEnd - tokenBegin + 1;

        new->strLen = tokenLen;
        new->str = (char*)malloc(new->strLen);

        strncpy(new->str, token, new->strLen);

        if(list == NULL)
        {
            list = new;
        }
        else
        {
            stringList* iter = list;
            while(iter->next != NULL)
                iter = iter->next;

            iter->next = new;
        }

        token = strtok(NULL, separator);
    }

    return list;
}

void stringList_clear(stringList* list)
{
    if(list == NULL) return;

    stringList* iter = list;
    while(iter != NULL)
    {
        stringList* next = iter->next;
        free(iter->str);
        free(iter);
        iter = next;
    }
}