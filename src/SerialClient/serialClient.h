#ifndef COMCLIENT_H
#define COMCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#define SERIALPORT      HANDLE
#define SERIALPARAMS    DCB
#define SERIALTIMEOUT   COMMTIMEOUTS

#elif __linux__

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#define SERIALPORT      int
#define SERIALPARAMS    struct termios
#define SERIALTIMEOUT   int

#endif

int serial_Open(SERIALPORT* port, char* portName);
int serial_Settings(SERIALPORT* port, SERIALPARAMS serialParams, SERIALTIMEOUT timeouts);
int serial_Write(SERIALPORT* port, char* msg);
int serial_Read(SERIALPORT* port, char* msg);
int serial_Close(SERIALPORT* port);




#endif  /* COMCLIENT_H */