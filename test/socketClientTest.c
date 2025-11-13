#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "socketClient.h"

int main(int argc, char* argv[])
{
    int ret = EXIT_SUCCESS;

    SOCKETCLIENTLIST* sockList = NULL;

    sockList = socketClientList_append(sockList, argv[1], argv[2], atoi(argv[3]));

    SOCKETCLIENT* sock = NULL;
    sock = socketClinetList_get(sockList, argv[1]);

    if(sock != NULL)
        socketClientList_free(sockList);
    else
        ret = EXIT_FAILURE;

    return ret;
}