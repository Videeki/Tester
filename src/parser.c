#include "parser.h"


int parser(PARSED* parsed, const char* path)
{
    FILE* fp = fopen(path, "r");

    if (fp == NULL)
    {
        perror("File Not Found!\n");
        return EXIT_FAILURE;
    }

    fseek(fp, 0L, SEEK_END);

    long int res = ftell(fp);
    
    fseek(fp, 0L, 0);

    res += 1;
    char* fileContent = (char*)malloc(res * sizeof(char));

    if(fileContent == NULL)
    {
        perror("Unsuccessfull memory allocation");
        return EXIT_FAILURE;
    }

    while(fgets(fileContent, res, fp))
    {
        if(fileContent[0] == '#' || fileContent[0] == '\n' || fileContent[0] == '\r') continue;
        else if(fileContent[0] == '[')
        {
            parsed->seqs = sequence_append(parsed->seqs, fileContent, 1);
        }
        else if(index_char(fileContent, '=') >= 0)
        {
            parsed->params = param_append(parsed->params, fileContent);
        }
        else
        {
            Sequences* iter = parsed->seqs;
            while(iter->next != NULL)
                iter = iter->next;

            parsed->keys = keys_append(parsed->keys, iter->sequence, fileContent, 1);
        }
    }

    fclose(fp);
    fp = NULL;

    free(fileContent);
    fileContent = NULL;

    return EXIT_SUCCESS;
}


Sequences* sequence_append(Sequences* list, const char* sequence, uint8_t copy)
{
    Sequences* new;
    new = (Sequences*)malloc(sizeof(Sequences));
    new->next = NULL;
    new->copy = copy;
    int seqNameLen = 1;
    while(sequence[seqNameLen] != ']')
        seqNameLen++;

    if(copy) new->sequence = (char*)malloc(seqNameLen * sizeof(char));

    strncpy(new->sequence, sequence + 1, seqNameLen - 1);
    new->sequence[seqNameLen - 1] = '\0';

    if(list == NULL)
    {
       list = new;
    }
    else
    {
        Sequences* iter = list;
        while(iter->next != NULL)
            iter = iter->next;

        iter->next = new;
    }

    return list;
}


int sequence_index(Sequences* list, const char* sequence)
{
    if(list == NULL) return -1;

    int i = 0;
    Sequences* iter = list;
    while(iter->next != NULL)
    {
        if(!strcmp(iter->sequence, sequence)) return i;
        iter = iter->next;
        i++;
    }
    if(!strcmp(iter->sequence, sequence)) return i;
    else return -1;
}


void sequence_free(Sequences* list)
{
    if(list == NULL) return;

    Sequences* iter = list;
    while(iter != NULL)
    {
        Sequences* next = iter->next;
        if(iter->copy) free(iter->sequence);
        free(iter);
        iter = next;
    }
}


Keys* keys_append(Keys* list, char* sequence, const char* key, uint8_t copy)
{
    Keys* new;
    new = (Keys*)malloc(sizeof(Keys));
    new->next = NULL;
    new->sequence = sequence;
    new->copy = copy;
    int keyNameLen = 0;
    while(key[keyNameLen] != '\n' && key[keyNameLen] != '\r')
        keyNameLen++;

    if(copy) new->key = (char*)malloc((keyNameLen + 1) * sizeof(char));

    strncpy(new->key, key, keyNameLen);
    new->key[keyNameLen] = '\0';

    if(list == NULL)
    {
       list = new;
    }
    else
    {
        Keys* iter = list;
        while(iter->next != NULL)
            iter = iter->next;

        iter->next = new;
    }

    return list;
}


void keys_free(Keys* list)
{
    if(list == NULL) return;

    Keys* iter = list;
    while(iter != NULL)
    {
        Keys* next = iter->next;
        if(iter->copy) free(iter->key);
        free(iter);
        iter = next;
    }
}


Parameters* param_append(Parameters* list, const char* param)
{
    /* az új elem létrehozása */
    Parameters* new;
    new = (Parameters*)malloc(sizeof(Parameters));
    new->next = NULL;
    
    int paramLen = strlen(param);
    for(int i = 0; i < paramLen; i++)
    {
        if(param[i] == '=')
        {
            int nameBegin = string_trim_before(param, 0);
            int nameEnd = string_trim_after(param, paramLen - i + 1);
            int nameLen = nameEnd - nameBegin + 1;
            
            new->name = (char*)malloc((nameLen + 1) * sizeof(char));
            strncpy(new->name, param + nameBegin, nameLen);
            new->name[nameLen] = '\0';
            

            int valueBegin = string_trim_before(param, i + 1);
            int valueEnd = string_trim_after(param, 0);
            int valueLen = valueEnd - valueBegin + 1;
            
            new->value = (char*)malloc((valueLen + 1) * sizeof(char));
            strncpy(new->value, param + valueBegin, valueLen);
            new->value[valueLen] = '\0';
            
            break;
        }
    }

    if(list == NULL)
    {
       list = new;
    }
    else
    {
        Parameters* iter = list;
        while(iter->next != NULL)
            iter = iter->next;

        iter->next = new;
    }

    return list;
}


char* get_parameter(Parameters* params, const char* parameter)
{
    Parameters* iter = params;
    while(iter->next != NULL)
    {
        if(!strcmp(iter->name, parameter))
        {
            return iter->value;
        }
        iter = iter->next;
    }
    if(!strcmp(iter->name, parameter))
    {
        return iter->value;
    }
    
    return NULL;
}


void param_free(Parameters* list)
{
    if(list == NULL) return;

    Parameters* iter = list;
    while(iter != NULL)
    {
        Parameters* next = iter->next;
        free(iter->name);
        free(iter->value);
        free(iter);
        iter = next;
    }
}
