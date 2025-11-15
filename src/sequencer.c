#include "sequencer.h"


int CMDProc(Parameters* list, char* cmd)
{
    static SOCKETCLIENTLIST* sockList = NULL;
    stringList* cmdList = NULL;
    cmdList = stringList_from_string(cmdList, cmd, "<-");

    printf("CMDPRoc\t-> ");

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
            printf("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(list, "PSIP"), get_parameter(list, "PSHost"));
            break;
        }

        case InitGateway:
        {
            printf("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(list, "GatewayIP"), get_parameter(list, "GatewayHost")); 
            break;
        }

        case SocketINIT:
        {
            printf("\e[38;2;0;255;0m%s\e[0m", cmdList->str);
            printf("\tHost Name: %s", cmdList->next->str);
            printf("\tHost Address: %s", cmdList->next->next->str);
            printf("\tHost Port: %s\n", cmdList->next->next->next->str);
        
            sockList = socketClientList_append(sockList, cmdList->next->str, cmdList->next->next->str, atoi(cmdList->next->next->next->str));
            if(sockList == NULL)
                fprintf(stderr, "Unsuccesfull sockList append");
        
            break;
        }

        case SocketCOMM:
        {
            int buffersize = 1024;
            char buffer[buffersize];

            memset(buffer, 0, buffersize);
        
            if(sockList == NULL)
                fprintf(stderr, "ERROR\t-> The sockList lost the value\n");

            SOCKETCLIENT* tmp = socketClinetList_get(sockList, cmdList->next->str);
            if(tmp != NULL)
            {
                printf("DEBUG\t-> Send message: %s to %s\n", cmdList->next->next->str, cmdList->next->str);
                socketClient_Send_Recieve(tmp, cmdList->next->next->str, buffer, buffersize);
            }
            else
                fprintf(stderr, "ERROR\t-> Undefined socket name: %s\n", cmdList->next->str);
            break;
        }

        case SocketCLEAN:
        {
            printf("Socklist close\n");
            if(sockList != NULL)
                socketClientListAll_free(sockList);
            break;
        }

        default:
        {
            printf("%s\n", cmd);
            break;
        }
    }

    stringList_clear(cmdList);

    return EXIT_SUCCESS;
}


void sequencer(Sequences* seqs, Keys* keys, Parameters* params, char* actSeq)
{
    if(keys == NULL) return;
    
    if(sequence_index(seqs, actSeq) >= 0)
    {
        Keys* keyiter = keys;
        while(keyiter->next != NULL)
        {
            if(!strcmp(keyiter->sequence, actSeq))
            {
                if(sequence_index(seqs, keyiter->key) >= 0)
                    sequencer(seqs, keyiter, params, keyiter->key);
                else
                    CMDProc(params, keyiter->key);
            }
            keyiter = keyiter->next;
        }

        if(!strcmp(keyiter->sequence, actSeq))
        {
            if(sequence_index(seqs, keyiter->key) >= 0)
                sequencer(seqs, keyiter, params, keyiter->key);
            else
                CMDProc(params, keyiter->key);
        }
    }
}
