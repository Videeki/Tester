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
