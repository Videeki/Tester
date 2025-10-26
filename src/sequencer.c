#include "sequencer.h"


int CMDProc(Parameters* list, char* cmd)
{
    printf("CMDPRoc -> ");
    switch(fastHash(cmd, strlen(cmd)))
    {
    case InitPS:
        printf("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(list, "PSIP"), get_parameter(list, "PSHost"));
        break;
    
    case InitGateway:
        printf("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(list, "GatewayIP"), get_parameter(list, "GatewayHost")); 
        break;

    default:
        printf("%s\n", cmd);
        break;
    }

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
