#include "sequencer.h"


int CMDProc(Parameters* list, char* cmd)
{
    static SOCKETCLIENTLIST* sockList = NULL;
    stringList* cmdList = NULL;
    cmdList = stringList_from_string(cmdList, cmd, "<-");

    printf("CMDPRoc -> ");

    uint32_t cmdIndex = 0;
    if(cmdList == NULL || cmdList->next == NULL)
        cmdIndex = fastHash(cmd, strlen(cmd));
    else
    {
        cmdIndex = fastHash(cmdList->str, cmdList->strLen);
        printf("DEBUG: %s: %d = %d\n", cmdList->str, cmdList->strLen, cmdIndex);
    }

    switch(cmdIndex)
    {
    case InitPS:
        printf("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(list, "PSIP"), get_parameter(list, "PSHost"));
        break;
    
    case InitGateway:
        printf("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(list, "GatewayIP"), get_parameter(list, "GatewayHost")); 
        break;

    case SocketINIT:
        printf("\e[38;2;0;255;0m%s\e[0m", cmdList->str);
        printf("\tHost Name: %s", cmdList->next->str);
        printf("\tHost Address: %s", cmdList->next->next->str);
        printf("\tHost Port: %s\n", cmdList->next->next->next->str);
        
        break;

    case SocketCLEAN:
        if(sockList != NULL)
            printf("Socklist\n");
            //socketClientList_free(sockList);
        break;

    default:
        printf("%s\n", cmd);
        break;
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
