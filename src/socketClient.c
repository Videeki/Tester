#include "socketClient.h"

SOCKETCLIENT* socketClient_Init(const char* ip_address, const int port)
{
    int ret = 0;

#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(ret != 0)
    {
        printf(SOCKETCLIENT_ERROR"WSAStartup failed: %d\n"SOCKETCLIENT_RESET, ret);
        return ret;
    }

    // Create a socket
    sock->ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock->ConnectSocket == INVALID_SOCKET)
    {
        fprintf(stderr, SOCKETCLIENT_ERROR "Socket creation failed: %d\n" SOCKETCLIENT_RESET, WSAGetLastError());
        WSACleanup();
        return -1;
    }

    // Define the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(sock->port); // Port number
    inet_pton(AF_INET, sock->ip_address, &serverAddr.sin_addr); // Server IP address

    // Connect to the server
    ret = connect(sock->ConnectSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret == SOCKET_ERROR)
    {
        printf(SOCKETCLIENT_ERROR"Connection failed: %d\n"SOCKETCLIENT_RESET, WSAGetLastError());
        closesocket(sock->ConnectSocket);
        WSACleanup();
        return ret;
    }

#elif __linux__
    ADDRINFO hints;
    ADDRINFO *host;

	memset(&hints, 0, sizeof(hints));		/* memset_s() */
	hints.ai_family = AF_INET;			/* IPv4 connection */
	hints.ai_socktype = SOCK_STREAM;	/* TCP, streaming */
	fprintf(stdout, "\e[38;2;0;0;255mSet all parameters\n\e[0m");
    char strPort[10] = "";
    sprintf(strPort, "%d", port);
	ret = getaddrinfo(ip_address, strPort, &hints, &host);
	fprintf(stdout, "\e[38;2;0;0;255mGetaddrinfo result: %d\n\e[0m", ret);
	if(ret != 0)
	{
		perror("Wrong address format\n");
		return NULL;
	}

	/* create a socket */
    SOCKETCLIENT* sock = (SOCKETCLIENT*)malloc(sizeof(SOCKETCLIENT));
	sock->ConnectSocket = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
	fprintf(stdout, "\e[38;2;0;0;255mSocket result: %d\n\e[0m", sock->ConnectSocket);
	if(sock->ConnectSocket == -1)
	{
		perror("Create socket failer\n");
        freeaddrinfo(host);
		return NULL;
	}

	/* connect and get the info */
	ret = connect(sock->ConnectSocket, host->ai_addr, host->ai_addrlen);
	fprintf(stdout, "\e[38;2;0;0;255mConnection result: %d\n\e[0m", ret);
	if(ret == -1)
	{
		perror("TCP client");
        freeaddrinfo(host);
	    close(sock->ConnectSocket);
		return NULL;
	}
    freeaddrinfo(host);

#endif

    return sock;
}


int socketClient_Send(SOCKETCLIENT* sock, char* message)
{
    int ret = 0;

#ifdef _WIN32
    ret = send(sock->ConnectSocket, message, strlen(message), 0);

    if(ret == SOCKET_ERROR)
    {
        printf(SOCKETCLIENT_ERROR"send failed with error: %d\n"SOCKETCLIENT_RESET, WSAGetLastError());
        closesocket(sock->ConnectSocket);
        WSACleanup();
        return ret;
    }

#elif __linux__
    ret = send(sock->ConnectSocket, message, strlen(message), 0);
	if(ret == -1)
	{
		perror("Send failed");
        close(sock->ConnectSocket);
		return ret;
	}    

#endif

    return ret;
}


int socketClient_Recieve(SOCKETCLIENT* sock, char* buffer, int bufferSize)
{
    int ret = 0;

#ifdef _WIN32
    puts("\t\t\tsocketClient_Recieve was called!\n");
    ret = recv(sock->ConnectSocket, buffer, sock->bufferSize - 1, 0);
    if(ret > 0)
    {
        buffer[ret] = '\0'; // Null-terminate the received data
    }
    else
    {
        printf(SOCKETCLIENT_ERROR"recieve failed with error: %d\n"SOCKETCLIENT_RESET, WSAGetLastError());
        closesocket(sock->ConnectSocket);
        WSACleanup();
        return SOCKET_ERROR;
    }

#elif __linux__
    ret = recv(sock->ConnectSocket, buffer, bufferSize, 0);
    if(ret > 0)
    {
        buffer[ret] = '\0';
	    printf("%s\n",buffer);
    }
	else
    {
        perror("Recv failed");
        close(sock->ConnectSocket);
		return ret;
    }

#endif

    return 0;
}


int socketClient_Send_Recieve(SOCKETCLIENT* sock, char* message, char* buffer, int bufferSize)
{
    int ret = 0;

#ifdef _WIN32
    // Send data to the server
    //const char* message = "Hello, Server!";
    ret = send(sock->ConnectSocket, message, strlen(message), 0);

    if(ret == SOCKET_ERROR)
    {
        printf(SOCKETCLIENT_ERROR"send failed with error: %d\n"SOCKETCLIENT_RESET, WSAGetLastError());
        closesocket(sock->ConnectSocket);
        WSACleanup();
        return ret;
    }

    // Receive data from the server
    int bytesReceived = recv(sock->ConnectSocket, buffer, sock->bufferSize - 1, 0);
    if(bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
    }
    else
    {
        printf(SOCKETCLIENT_ERROR"recieve failed with error: %d\n"SOCKETCLIENT_RESET, WSAGetLastError());
        closesocket(sock->ConnectSocket);
        WSACleanup();
        return SOCKET_ERROR;
    }

#elif __linux__
    ret = send(sock->ConnectSocket, message, strlen(message), 0);
	if(ret == -1)
	{
		perror("Send failed");
        close(sock->ConnectSocket);
		return ret;
	}    

    ret = recv(sock->ConnectSocket, buffer, bufferSize, 0);
    if(ret > 0)
    {
        buffer[ret] = '\0';
	    printf("%s\n",buffer);
    }
	else
    {
        perror("Recv failed");
        close(sock->ConnectSocket);
		return ret;
    }


#endif

    return EXIT_SUCCESS;
}


int socketClient_Deinit(SOCKETCLIENT* sock)
{
#ifdef _WIN32
    // Clean up
    closesocket(sock->ConnectSocket);
    WSACleanup();

#elif __linux__
    /* close-up */
	close(sock->ConnectSocket);
    free(sock);

#endif

    return 0;
}


//int socketClient_Compact(SOCKETCLIENT* sock, char* message, char* buffer, int bufferSize)
//{
//    int ret = 0; 
//
//#ifdef _WIN32
//
//#elif __linux__
//    ADDRINFO hints;
//    ADDRINFO *host;
//
//	memset(&hints, 0, sizeof(hints));		/* memset_s() */
//	hints.ai_family = AF_INET;			/* IPv4 connection */
//	hints.ai_socktype = SOCK_STREAM;	/* TCP, streaming */
//	fprintf(stdout, "\e[38;2;0;0;255mSet all parameters\n\e[0m");
//    char strPort[10] = "";
//    sprintf(strPort, "%d", sock->port);
//	ret = getaddrinfo(sock->ip_address, strPort, &hints, &host);
//	fprintf(stdout, "\e[38;2;0;0;255mGetaddrinfo result: %d\n\e[0m", ret);
//	if(ret != 0)
//	{
//		perror("Wrong address format\n");
//		return -1;
//	}
//
//	/* create a socket */
//	sock->ConnectSocket = socket(host->ai_family, host->ai_socktype, host->ai_protocol);
//	fprintf(stdout, "\e[38;2;0;0;255mSocket result: %d\n\e[0m", sock->ConnectSocket);
//	if(sock->ConnectSocket == -1)
//	{
//		perror("Create socket failer\n");
//        freeaddrinfo(host);
//		return -1;
//	}
//
//	/* connect and get the info */
//	ret = connect(sock->ConnectSocket, host->ai_addr, host->ai_addrlen);
//	fprintf(stdout, "\e[38;2;0;0;255mConnection result: %d\n\e[0m", ret);
//	if(ret == -1)
//	{
//		perror("TCP client");
//        freeaddrinfo(host);
//	    close(sock->ConnectSocket);
//		return -1;
//	}
//    freeaddrinfo(host);
//
//    ret = send(sock->ConnectSocket, message, strlen(message), 0);
//	if(ret == -1)
//	{
//		perror("Send failed");
//        close(sock->ConnectSocket);
//		return ret;
//	}    
//
//    ret = recv(sock->ConnectSocket, buffer, bufferSize, 0);
//    if(ret > 0)
//    {
//        buffer[ret] = '\0';
//	    printf("%s\n",buffer);
//    }
//	else
//    {
//        perror("Recv failed");
//        close(sock->ConnectSocket);
//		return ret;
//    }
//
//    close(sock->ConnectSocket);
//
//#endif
//
//    return EXIT_SUCCESS;
//}


SOCKETCLIENTLIST* socketClientList_append(SOCKETCLIENTLIST* list, const char* name, const char* ip_address, const int port)
{
    SOCKETCLIENTLIST* new;
    new = (SOCKETCLIENTLIST*)malloc(sizeof(SOCKETCLIENTLIST));
    
    if(new == NULL) return NULL;

    new->next = NULL;

    int nameLen = strlen(name);
    new->name = (char*)malloc(nameLen * sizeof(char));
    strncpy(new->name, name, nameLen);

    new->sock = socketClient_Init(ip_address, port);
    if(new->sock == NULL)
    {
        free(new->name);
        free(new);
        return NULL;
    }

    if(list == NULL)
    {
       list = new;
    }
    else
    {
        SOCKETCLIENTLIST* iter = list;
        while(iter->next != NULL)
            iter = iter->next;

        iter->next = new;
    }

    return list;
}


SOCKETCLIENT* socketClinetList_get(SOCKETCLIENTLIST* list, const char* name)
{
    SOCKETCLIENTLIST* iter = list;
    while(iter->next != NULL)
    {
        if(!strcmp(iter->name, name))
        {
            return iter->sock;
        }
        iter = iter->next;
    }

    if(!strcmp(iter->name, name))
    {
        return iter->sock;
    }
    
    return NULL;
}


void socketClientList_free(SOCKETCLIENTLIST* list)
{
    if(list == NULL) return;

    SOCKETCLIENTLIST* iter = list;
    while(iter != NULL)
    {
        SOCKETCLIENTLIST* next = iter->next;
        free(iter->name);
        free(iter->sock);
        free(iter);
        iter = next;
    }
}



#ifdef RUNABLE
//  Windows:    gcc -Wall -D RUNABLE socketClient.c -o .\bin\SocketClient -lWs2_32
//  Linux:      gcc -Wall -D RUNABLE socketClient.c -o ./bin/SocketClient
int main(int argc, char* argv[])
{
    int run = 1;
    char cmd;
    int index = 1;
    uint8_t counter = 0;
    char ip_address[16] = "127.0.0.1";
    SOCKETCLIENT sock;
    sock.ip_address = ip_address;
    sock.port = 54000;
    sock.bufferSize = 4096;

    char message[4096];

    if(argc < 2)
    {
        puts(SOCKETCLIENT_ERROR "No any arguments added\n" SOCKETCLIENT_RESET);
        return -1;
    }

    do
    {
        if(index >= argc) return 0;
        
        if(counter < strlen(argv[index]))
        {
            cmd = argv[index][counter];
        }
        else if(counter == 2)
        {
            char* temp = (char*)argv[index][counter] + 2;
            if(!strcmp(temp, "ip")) cmd = 'i';
            else if(!strcmp(temp, "port")) cmd = 'p';
            else if(!strcmp(temp, "buffersize")) cmd = 'b';
            else if(!strcmp(temp, "connect")) cmd = 'c';
            else if(!strcmp(temp, "disconnect")) cmd = 'd';
            else if(!strcmp(temp, "send")) cmd = 's';
            else if(!strcmp(temp, "recieve")) cmd = 'r';
            else if(!strcmp(temp, "talk")) cmd = 't';
            else if(!strcmp(temp, "quit")) cmd = 'q';
            else cmd = '0';
        }
        else
        {
            puts(SOCKETCLIENT_ERROR"Wrong param added!\n"SOCKETCLIENT_RESET);
            return -2;
        }
        
        switch (cmd)
        {
            case '-':
            {
                counter++;
                break;
            }
            case 'i':
            {
                index++;
                if(strlen(argv[index]) > 16)
                {
                    puts(SOCKETCLIENT_ERROR"Too long IP V4 address added\n"SOCKETCLIENT_RESET);
                    return -2;
                }
                strncpy(ip_address, argv[index], 16);
                printf("IP address is set: %s\n", ip_address);
                sock.ip_address = ip_address;
                counter = 0;
                index++;
                break;
            }
            case 'p':
            {
                index++;
                sock.port = atoi(argv[index]);
                printf("Port is set: %d\n", sock.port);
                counter = 0;
                index++;
                break;
            }
            case 'b':
            {
                index++;
                sock.bufferSize = atoi(argv[index]);
                printf("Buffer size is set: %d\n", sock.bufferSize);
                counter = 0;
                index++;
                break;
            }
            case 'c':
            {
                socketClient_Init(&sock);
                index++;
                break;
            }
            case 's':
            {
                index++;
                socketClient_Send(&sock, argv[index]);
                index++;
                break;
            }
            case 'r':
            {
                socketClient_Recieve(&sock, message, sock.bufferSize);
                printf("\e[38;2;0;255;0m%s\n\e[0m", message);
                index++;
                break;
            }
            case 't':
            {
                index++;
                socketClient_Send_Recieve(&sock, argv[index], message, sock.bufferSize);
                printf("\e[38;2;0;255;0m%s\n\e[0m", message);
                index++;
                break;
            }
            case 'd':
            {
                socketClient_Deinit(&sock);
                index++;
                break;
            }
            case 'q':
            {
                run = 0;
                break;
            }
            default:
            {
                puts(SOCKETCLIENT_WARNING"Invalid command\n"SOCKETCLIENT_RESET);
                counter = 0;
                index++;
                break;
            }
        }
        //printf("%s\n", fullName);
    }while(run);

    return 0;
}

#endif
