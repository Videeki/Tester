#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>


#define MSGSIZE 4096
#define FILENAME "writeshmem.c"

typedef struct buffer
{
    int status;
    pid_t pid;
    char msg[MSGSIZE];
}BUFFER_t;

#define BUFFERSIZE sizeof(BUFFER_t)

BUFFER_t* attach_memory_block(char* filename, int size);
bool detach_memory_block(BUFFER_t* block);
bool destroy_memory_block(char* filename);


#endif  /* SHARED_MEMORY_H */