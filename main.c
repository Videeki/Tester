#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>

#include "includes/sharedMemory.h"
#include "includes/interface.h"
#include "includes/parser.h"
#include "includes/sequencer.h"
#include "includes/logger.h"
//#include "includes/memManager.h"


static BUFFER_t* block;
#define MAXSEQNAMELEN 255
static char startSequence[MAXSEQNAMELEN] = {0};


int main(int argc, char* argv[])
{
    int cmd = -1;
    int optionIndex = 0;
    int verbose_flag = 0;
    const struct option longOptions[] = {
        {"Init",        no_argument,            0,  'I'},
        {"Parse",       required_argument,      0,  'P'},
        {"Launch",      required_argument,      0,  'L'},
        {"Quit",        no_argument,            0,  'Q'},
        {"info",        no_argument,            0,  'i'},
        {"stop",        no_argument,            0,  's'},
        {"pause",       no_argument,            0,  'p'},
        {"lstall",      no_argument,            0,  -2},
        {"lstseq",      no_argument,            0,  -3},
        {"addSeq",      required_argument,      0,  -4},
        {"addStp",      required_argument,      0,  -5},
        {"delSeq",      required_argument,      0,  -6},
        {"delStp",      required_argument,      0,  -7},
        {"savefl",      required_argument,      0,  -8},
        {"loadfl",      no_argument,            0,  -9},
        /* These options set a flag. */
        {"verbose",     no_argument,       &verbose_flag, 1},
        {"brief",       no_argument,       &verbose_flag, 0},
        {0, 0, 0, 0}
    };

    while((cmd = getopt_long(argc, argv, "IP:L:Qisp", longOptions, &optionIndex)) != -1)
    {
        switch(cmd)
        {
            case 'I':
            {
                block = attach_memory_block(FILENAME, BUFFERSIZE);
                if(block != NULL)
                {
                    block->data = (TESTER*)malloc(sizeof(TESTER));
                    block->data->seqs = NULL;
                    block->data->keys = NULL;
                    block->data->params = NULL;
                }
                else
                {
                    perror("Failed to attach memory block");
                    return EXIT_FAILURE;
                }

                break;
            }
            case 'L':
            {
                if(block == NULL)
                {
                    perror("Shared memory block has not been initialized. Please run with -I/--Init option first.\n");
                    return EXIT_FAILURE;
                }

                char cmd[4082] = {0};
                if(optarg != NULL)
                    strncpy(startSequence, optarg, MAXSEQNAMELEN);
                else
                    if(block->data->seqs->sequence != NULL)
                        strncpy(startSequence, block->data->seqs->sequence, MAXSEQNAMELEN);
                    else
                    {
                        perror("No start sequence provided\n");
                        return EXIT_FAILURE;
                    }

                sprintf(cmd, "./sequencer %s %ld %s &", FILENAME, BUFFERSIZE, startSequence);
                system(cmd);
                detach_memory_block(block);
                break;
            }
            case 'i':
            {
                if(block == NULL)
                {
                    printf("Shared memory block has not been initialized. To initialize, run with -I/--Init option.\n");
                }
                else
                {
                    printf("PID: %d\n", block->pid);
                    printf("Status: %d\n", block->status);
                }
                break;
            }
            case 'Q':
            {
                log_path_free();

                if(block == NULL) return EXIT_SUCCESS;
                
                if(block->pid != 0)
                {
                    kill(block->pid, SIGTERM);
                }
            
                param_free(block->data->params);
                keys_free(block->data->keys);
                sequence_free(block->data->seqs);
                free(block->data);

                block->pid = 0;
                block->status = 0;

                detach_memory_block(block);
                destroy_memory_block(FILENAME);
                printf("Resources have been cleaned up. Exiting.\n");
                break;
            }
            case 'P':
            {
                if(block == NULL)
                {
                    perror("Shared memory block has not been initialized. Please run with -I/--Init option first.\n");
                    return EXIT_FAILURE;
                }

                int ret = parser(block->data, optarg);
                if(ret)
                {
                    perror("Unsuccessfull parsing");
                    return EXIT_FAILURE;
                }
                detach_memory_block(block);
                break;
            }
            case 's':
            {
                break;
            }
            default:
                break;
        }
    }

    return 0;
}