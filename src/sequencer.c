#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include "logger.h"
#include "parser.h"
#include "hashCalc.h"
#include "stringUtils.h"
#include "socketClient.h"
#include "serialClient.h"
#include "sharedMemory.h"

#include "sequencer.h"

static SOCKETCLIENTLIST* sockList = NULL;


BUFFER_t* buffer = NULL;

static void testerMemFree(int sigNum)
{
    if(sockList != NULL)
    {
        socketClientListAll_free(sockList);
        sockList = NULL;
    }

    buffer->pid = 0;
    detach_memory_block(buffer);
}


//int CMDProc(Parameters* list, char* cmd);
int CMDProc(TESTER* self, char* cmd)
{
    stringList* cmdList = NULL;
    cmdList = stringList_from_string(cmdList, cmd, "<-");

    uint32_t cmdIndex = 0;
    if(cmdList == NULL || cmdList->next == NULL)
        cmdIndex = fastHash(cmd, strlen(cmd));
    else
    {
        cmdIndex = fastHash(cmdList->str, cmdList->strLen);
    }

    switch(cmdIndex)
    {
        case InitPS:
        {
            LOG("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(self->params, "PSIP"), get_parameter(self->params, "PSHost"));
            break;
        }

        case InitGateway:
        {
            LOG("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(self->params, "GatewayIP"), get_parameter(self->params, "GatewayHost")); 
            break;
        }

        case SocketINIT:
        {
            LOG("\e[38;2;0;255;0m%s\e[0m    \
                 \tHost Name: %s    \
                 \tHost Address: %s \
                 \tHost Port: %s\n",
                 cmdList->str,
                 cmdList->next->str,
                 cmdList->next->next->str,
                 cmdList->next->next->next->str);
        
            sockList = socketClientList_append(sockList, cmdList->next->str, cmdList->next->next->str, atoi(cmdList->next->next->next->str));
            if(sockList == NULL)
                ERROR("Unsuccesfull sockList append");
        
            break;
        }

        case SocketCOMM:
        {
            int buffersize = 1024;
            char buffer[buffersize];

            memset(buffer, 0, buffersize);
        
            if(sockList == NULL)
                ERROR("The sockList lost the value\n");

            SOCKETCLIENT* tmp = socketClinetList_get(sockList, cmdList->next->str);
            if(tmp != NULL)
            {
                DEBUG("Send message: %s to %s\n", cmdList->next->next->str, cmdList->next->str);
                socketClient_Send_Recieve(tmp, cmdList->next->next->str, buffer, buffersize);
            }
            else
                ERROR("Undefined socket name: %s\n", cmdList->next->str);
            break;
        }

        case SocketCLEAN:
        {
            LOG("Socklist close\n");
            if(sockList != NULL)
                //socketClientListAll_free(sockList);
                socketClientList_free(sockList, cmdList->next->str);
            break;
        }

        case LogINIT:
        {
            LOG("Init log path: %s\n", cmdList->next->str);
            log_path_init(cmdList->next->str);
            break;
        }

        case LogFREE:
        {
            LOG("Free log path\n");
            log_path_free();
            break;
        }

        default:
        {
            LOG("%s\n", cmd);
            break;
        }
    }

    stringList_clear(cmdList);

    return EXIT_SUCCESS;
}


void sequencer(TESTER* self, const char* actSeq)
{
    if(self->keys == NULL) return;
    
    if(sequence_index(self->seqs, actSeq) >= 0)
    {
        Keys* keyiter = self->keys;
        while(keyiter->next != NULL)
        {
            if(!strcmp(keyiter->sequence, actSeq))
            {
                if(sequence_index(self->seqs, keyiter->key) >= 0)
                    sequencer(self, keyiter->key);
                else
                    CMDProc(self, keyiter->key);
            }
            keyiter = keyiter->next;
        }

        if(!strcmp(keyiter->sequence, actSeq))
        {
            if(sequence_index(self->seqs, keyiter->key) >= 0)
                sequencer(self, keyiter->key);
            else
                CMDProc(self, keyiter->key);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc > 3)
    {
        perror("Not enough parameter added\n");
        return EXIT_FAILURE;
    }
    signal(SIGSEGV, testerMemFree);
    signal(SIGINT, testerMemFree);

    buffer = attach_memory_block(argv[1], atoi(argv[2]));
    if(buffer == NULL)
    {
        perror("Memory block attach failed\n");
        return EXIT_FAILURE;
    }

    if(sequence_index(buffer->data->seqs, argv[3]) < 0)
    {
        perror("Invalid sequence provided\n");
        return EXIT_FAILURE;
    }

    buffer->pid = getpid();
    sequencer(buffer->data, argv[3]);

    socketClientListAll_free(sockList);
    buffer->pid = 0;
    detach_memory_block(buffer);

    return EXIT_SUCCESS;
}