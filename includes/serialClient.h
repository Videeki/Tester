#ifndef COMCLIENT_H
#define COMCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum stopBits {ONE, ONE_AND_HALF, TWO};
enum parity {NONE, ODD, EVEN, MARK, SPACE};

typedef struct serialparams
{
    int BaudRate;           // Setting BaudRate
    int ByteSize;           // Setting ByteSize
    enum stopBits StopBits; // Setting StopBits
    enum parity Parity;     // Setting Parity
}SerialParams;

#define SERIALPARAMS    SerialParams

#ifdef _WIN32
#include <Windows.h>
#define SERIALPORT      HANDLE
#define SERIALTIMEOUT   COMMTIMEOUTS

#elif __linux__
#include <fcntl.h>      // Contains file controls like O_RDWR
#include <errno.h>      // Error integer and strerror() function
#include <termios.h>    // Contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()
#define SERIALPORT      int
#define SERIALTIMEOUT   int

#endif

int serial_Open(SERIALPORT* port, char* portName);
int serial_Settings(SERIALPORT* port, SERIALPARAMS serialParams, SERIALTIMEOUT timeouts);
int serial_Write(SERIALPORT* port, char* msg);
int serial_Read(SERIALPORT* port, char* msg);
int serial_Close(SERIALPORT* port);




#endif  /* COMCLIENT_H */