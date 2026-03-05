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
}

int main(int argc, char* argv[])
{
    if(argc < MINARGC)
    {
        fprintf(stderr, "Use argument: %s <argument>\n", __FILE__);
        return EXIT_FAILURE;
    }

    BUFFER_t* block = attach_memory_block(FILENAME, BUFFERSIZE);
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
    else if(0 == forkID)    //Child process
    {
        signal(SIGSEGV, alert);
        block->pid = getpid();
        do
        {
            printf("Reading: \"%s\"\n", block->msg);
            raise(SIGSTOP);
        } while(strcmp("__STOP__", block->msg));

        block->pid = 0;
        printf("So long, and thanks for all the fish\n");
        detach_memory_block(block);

        if(destroy_memory_block(FILENAME))
        {
            printf("Distroyed block: %s\n", FILENAME);
        }
        else
        {
            printf("Could not destroy block: %s\n", FILENAME);
        }

        printf("Stop fork");
        return EXIT_SUCCESS;
    }
    else    //Main process
    {
        int option;
        while((option = getopt(argc, argv, "prsm:v::")) != -1)   //get option from the getopt() method
        {
            switch(option)
            {
                case 'r':
                {
                    block->pid = 0;
                    block->status = 0;
                    memset(block->msg, 0, 4096); 
                    break;
                }
                case 's':
                {
                    strncpy(block->msg, STOPMSG, STOPMSGSIZE);
                    kill(block->pid, SIGCONT);
                    break;
                }
                case 'm':
                {
                    strncpy(block->msg, optarg, MSGSIZE);
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