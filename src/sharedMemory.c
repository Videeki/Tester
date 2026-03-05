#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include "sharedMemory.h"

#define IPC_RESULT_ERROR (-1)

static int get_shared_block(char* filename, int size)
{
    FILE *file;
    if((file = fopen(filename, "r")))
    {
        fclose(file);
    }
    else if((file = fopen(filename, "w")))
    {
        char buff[BUFFERSIZE];
        memset(buff, ' ', BUFFERSIZE);
        fprintf(file, buff);
        fclose(file);
    }
    else
    {
        perror("SharedMemory creation error");
    }

    key_t key = ftok(filename, 0);
    if(IPC_RESULT_ERROR == key)
    {
        perror("ERROR: ftok");
        return IPC_RESULT_ERROR;
    }

    int ret = shmget(key, size, 0644 | IPC_CREAT);

    return ret;
}

BUFFER_t* attach_memory_block(char* filename, int size)
{
    int shared_block_id = get_shared_block(filename, size);

    if(IPC_RESULT_ERROR == shared_block_id)
    {
        fprintf(stderr, "ERROR: Shared block ID of get_shared_block is %d\n", shared_block_id);
        return NULL;
    }

    BUFFER_t* result = shmat(shared_block_id, NULL, 0);
    if(IPC_RESULT_ERROR == result->status)
    {
        perror("ERROR: shmat");
        return NULL;
    }

    return result;
}

bool detach_memory_block(BUFFER_t* block)
{
    return (shmdt(block) != IPC_RESULT_ERROR);
}

bool destroy_memory_block(char* filename)
{
    int shared_block_id = get_shared_block(filename, 0);

    if(shared_block_id == IPC_RESULT_ERROR) return NULL;

    return (shmctl(shared_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}
