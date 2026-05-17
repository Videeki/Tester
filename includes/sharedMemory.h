#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/types.h>
#include <stdbool.h>
#include "interface.h"

#ifndef FILENAME
#define FILENAME ".sharedMemoryFile"
#endif /* FILENAME */


typedef struct buffer
{
    int status;
    pid_t pid;
    TESTER* data;
}BUFFER_t;


#define BUFFERSIZE (sizeof(TESTER*))

BUFFER_t* attach_memory_block(char* filename, int size);
bool detach_memory_block(BUFFER_t* block);
bool destroy_memory_block(char* filename);


#endif  /* SHARED_MEMORY_H */