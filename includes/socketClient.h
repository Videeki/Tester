#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <glib.h>

#define WIN32_LEAN_AND_MEAN
#define SOCKETCLIENT_ERROR "\e[38;2;255;0;0m"
#define SOCKETCLIENT_WARNING "\e[38;2;255;255;0m"
#define SOCKETCLIENT_RESET "\e[0m"

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")     //in case gcc use the "-lws2_32"

typedef struct socketclient
{
    SOCKET ConnectSocket;
    char* ip_address;
    int port;
    int bufferSize;
}SOCKETCLIENT;


#elif __linux__
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

#define ADDRINFO struct addrinfo

typedef struct socketclient
{
    int ConnectSocket;
    char* ip_address;
    int port;
    int bufferSize;
}SOCKETCLIENT;

#endif  /* OS Selector */

typedef struct socketclientlist SOCKETCLIENTLIST;

struct socketclientlist
{
    char* name;
    SOCKETCLIENT* sock;
    SOCKETCLIENTLIST* next;
};


int socketClient_Init(SOCKETCLIENT* sock);
int socketClient_Send(SOCKETCLIENT* sock, char* message);
int socketClient_Recieve(SOCKETCLIENT* sock, char* buffer, int bufferSize);
int socketClient_Send_Recieve(SOCKETCLIENT* sock, char* message, char* buffer, int bufferSize);
int socketClient_Deinit(SOCKETCLIENT* sock);
int socketClient_Compact(SOCKETCLIENT* sock, char* message, char* buffer, int bufferSize);

SOCKETCLIENTLIST* socketClientList_append(SOCKETCLIENTLIST* list, const SOCKETCLIENT* sock, const char* name);
SOCKETCLIENT* socketClinetList_get(SOCKETCLIENTLIST* list, const char* name);
void socketClientList_free(SOCKETCLIENTLIST* list);

#endif  /* SOCKETCLIENT_H */