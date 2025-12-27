#include "sequencer.h"

//int CMDProc(Parameters* list, char* cmd);
int CMDProc(TESTER* data, char* cmd)
{
    static SOCKETCLIENTLIST* sockList = NULL;
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
            LOG("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(data->desc->params, "PSIP"), get_parameter(data->desc->params, "PSHost"));
            break;
        }

        case InitGateway:
        {
            LOG("\e[38;2;0;255;0m%s -> IP:%s, Host:%s\e[0m\n", cmd, get_parameter(data->desc->params, "GatewayIP"), get_parameter(data->desc->params, "GatewayHost")); 
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
        
            data->sockList = socketClientList_append(data->sockList, cmdList->next->str, cmdList->next->next->str, atoi(cmdList->next->next->next->str));
            if(data->sockList == NULL)
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
            if(data->sockList != NULL)
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


//void sequencer(Sequences* seqs, Keys* keys, Parameters* params, char* actSeq)
void sequencer(TESTER* data, const char* actSeq)
{
    if(data->desc->keys == NULL) return;
    
    if(sequence_index(data->desc->seqs, actSeq) >= 0)
    {
        Keys* keyiter = data->desc->keys;
        while(keyiter->next != NULL)
        {
            if(!strcmp(keyiter->sequence, actSeq))
            {
                if(sequence_index(data->desc->seqs, keyiter->key) >= 0)
                    sequencer(data, keyiter->key);
                else
                    CMDProc(data, keyiter->key);
            }
            keyiter = keyiter->next;
        }

        if(!strcmp(keyiter->sequence, actSeq))
        {
            if(sequence_index(data->desc->seqs, keyiter->key) >= 0)
                sequencer(data, keyiter->key);
            else
                CMDProc(data, keyiter->key);
        }
    }
}
