#ifndef PARSER_H
#define PARSER_H

#include "interface.h"


int parser(TESTER* self, const char* path);

Sequences* sequence_append(Sequences* list, const char* sequence, uint8_t copy);
int sequence_index(Sequences* list, const char* sequence);
void sequence_free(Sequences* list);

Keys* keys_append(Keys* list, char* sequence, const char* key, uint8_t copy);
void keys_free(Keys* list);

Parameters* param_append(Parameters* list, const char* param);
char* get_parameter(Parameters* list, const char* parameter);
void param_free(Parameters* list);

#endif  /* PARSER_H */