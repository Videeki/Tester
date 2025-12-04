#include "stringUtils.h"


void string_trim(char* str)
{
    size_t len;
    int i;

    if(str == NULL) return;

    len = strlen(str);
    if (len == 0) return;

    /* trim right */
    for (i = len - 1; (i >= 0) && (str[i] > 32); i--) {}
    str[i + 1] = '\0';
    
    /* trim left */
    for (i = 0; (str[i] != '\0') && (str[i] > 32); i++) {}
    memmove(str, &str[i], len - i);
    str[len - i] = '\0';
}


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


int string_char_index(char* str, char chr)
{
    for(int i = 0; str[i] != '\0'; i++)
        if(str[i] == chr)
            return i;

    return -1;
}


PATH string_path_init(const char* path)
{
    int pathLen = strlen(path);
    
    PATH pathInit = (PATH)malloc(pathLen * sizeof(char));
    strncpy(pathInit, path, pathLen);
    pathInit[pathLen] = '\0';

    return pathInit;
}


PATH string_path_append(PATH self, const char* pathItem)
{
    if(self == NULL) return NULL;
    if(pathItem == NULL) return self;

    int selfLen = strlen(self);
    int pathItemLen = strlen(pathItem);
    int newSelfLen = selfLen + 1 + pathItemLen;

    self = (PATH)realloc(self, newSelfLen * sizeof(char));
    
    #ifdef _WIN32
        self[selfLen] = '\\';
    #else
        self[selfLen] = '/';
    #endif

    self[selfLen + 1] = '\0';
    strncat(self, pathItem, pathItemLen);

    self[newSelfLen] = '\0';

    return self;
}


PATH string_path_append_items(PATH self, ...)
{
    va_list args;

    va_start(args, self);

    char* iter = NULL;
    while((iter = va_arg(args, PATH)) != NULL)
        self = string_path_append(self, iter);

    va_end(args);

    return self;
}


void string_path_free(PATH self)
{
    if(self != NULL)
        free(self);
}


XPATH string_xpath_init(const char* xpath)
{
    int xpathLen = strlen(xpath);
    
    XPATH xpathInit = (XPATH)malloc(xpathLen * sizeof(char));
    strncpy(xpathInit, xpath, xpathLen);
    xpathInit[xpathLen] = '\0';

    return xpathInit;
}


XPATH string_xpath_append(XPATH self, const char* xpathItem)
{
    if(self == NULL) return NULL;
    if(xpathItem == NULL) return self;

    int selfLen = strlen(self);
    int xpathItemLen = strlen(xpathItem);
    int newSelfLen = selfLen + 1 + xpathItemLen;

    self = (XPATH)realloc(self, newSelfLen * sizeof(char));

    self[selfLen] = '/';
    self[selfLen + 1] = '\0';
    strncat(self, xpathItem, xpathItemLen);

    self[newSelfLen] = '\0';

    return self;
}


XPATH string_xpath_append_items(XPATH self, ...)
{
    va_list args;

    va_start(args, self);

    char* iter = NULL;
    while((iter = va_arg(args, XPATH)) != NULL)
        self = string_xpath_append(self, iter);

    va_end(args);

    return self;
}


void string_xpath_free(XPATH self)
{
    if(self != NULL)
        free(self);
}


stringList* stringList_from_string(stringList* list, char* str, char* separator)
{
    char* token = strtok(str, separator);

    while(token != NULL)
    {   
        //Trim the token
        int tokenBegin = string_trim_before(token, 0);
        int tokenEnd = string_trim_after(token, 0);
        int tokenLen = tokenEnd - tokenBegin + 1;

        stringList* new = (stringList*)malloc(sizeof(stringList));
        new->next = NULL;
        new->strLen = tokenLen;
        new->str = (char*)malloc(new->strLen);

        strncpy(new->str, token + tokenBegin, new->strLen);
        new->str[new->strLen] = '\0';

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


char* stringList_select_string(stringList* list, int index)
{
    if(list == NULL) return NULL;

    stringList* iter = list;
    for(int i = 0; i < index; ++i)
        iter = iter->next;

    return iter->str;
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