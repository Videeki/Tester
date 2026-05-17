#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "sharedMemory.h"

#define MINARGC 2
#define STOPMSG "__STOP__"
#define STOPMSGSIZE 9


void alert(int sigID)
{
    perror("Segmentation fault signaled\n");
    destroy_memory_block(FILENAME);
    abort();
}

int main(int argc, char* argv[])
{
    if(argc < MINARGC)
    {
        fprintf(stderr, "Use argument: %s <argument>\n", __FILE__);
        return EXIT_FAILURE;
    }

    signal(SIGSEGV, alert);
    BUFFER_t* block = attach_memory_block(FILENAME, (sizeof(int) + sizeof(pid_t) + MSGSIZE));
    if(block == NULL)
    {
        perror("ERROR: could not get block\n");
        return EXIT_FAILURE;
    }

    int forkID = 1;
    if(0 == block->pid)
    {
        printf("Start fork\n");
        forkID = fork();
    }

    if(-1 == forkID)
    {
        perror("ERROR: fork\n");
        return EXIT_FAILURE;
    }
    else if(0 == forkID)    /* Child process */
    {
        block->pid = getpid();
        printf("Child started\n");
        char* msg;
        do
        {
            msg = (char*)block->data;
            printf("%s\n", msg);
            raise(SIGSTOP);
        } while(strcmp("__STOP__", msg));

        block->pid = 0;
        detach_memory_block(block);

        if(destroy_memory_block(FILENAME))
        {
            printf("Distroyed block: %s\n", FILENAME);
        }
        else
        {
            printf("Could not destroy block: %s\n", FILENAME);
        }

        return EXIT_SUCCESS;
    }
    else    //Main process
    {
        int option;
        while((option = getopt(argc, argv, "iprsm:v::")) != -1)   //get option from the getopt() method
        {
            switch(option)
            {
                case 'i':
                {
                    block->pid = 0;
                    block->status = 0;
                    block->data = NULL;
                    break;
                }
                case 'r':
                {
                    
                    break;
                }
                case 's':
                {
                    block->data = (void*)STOPMSG;
                    kill(block->pid, SIGCONT);
                    break;
                }
                case 'm':
                {
                    block->data = (void*)optarg;
                    kill(block->pid, SIGCONT);
                    break;
                }
                case 'p':
                {
                    printf("Status:\t%d\nPID:\t%d\n", block->status, block->pid);
                    break;
                }
                case 'v':
                {
                    printf("Verbose mode\n");
                    break;
                }
                case '?':
                {
                    printf("Unknown option: %c\n", optopt);
                    break;
                }
                default:
                {
                    printf("Writting: \"%s\"\n", argv[1]);
                }
            }
        }
    }

    detach_memory_block(block);

    return EXIT_SUCCESS;
}