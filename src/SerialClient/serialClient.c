#include "serialClient.h"


int serial_Open(SERIALPORT* port, char* portName)
{
    int ret = 0;

#ifdef _WIN32
    char ComPortName[10] = "\\\\.\\";      // Name of the Serial port(May Change) to be opened,
    strcat(ComPortName, portName);
    int comportLen = strlen(ComPortName);
    ComPortName[comportLen] = '\0';
    BOOL Status;                           // Status of the various operations 
    
    /*---------------------------------- Opening the Serial Port -------------------------------------------*/
    
    //port = CreateFile("\\\\.\\COM8",                  // Name of the Port to be Opened
    port = CreateFile( ComPortName,                  // Name of the Port to be Opened
                            GENERIC_READ | GENERIC_WRITE, // Read/Write Access
                            0,                            // No Sharing, ports cant be shared
                            NULL,                         // No Security
                            OPEN_EXISTING,                // Open existing port only
                            0,                            // Non Overlapped I/O
                            NULL);                        // Null for Comm Devices

    if(port == INVALID_HANDLE_VALUE)
    {
        printf("\n    Error! - Port %s can't be opened\n", ComPortName);
        CloseHandle(port);
        return -1;
    }


    /*------------------------------- Setting the Parameters for the SERIALPORT ------------------------------*/
    
    DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(port, &dcbSerialParams);      //retreives  the current settings

    if(Status == FALSE)
    {
        printf("\n    Error! in GetCommState()");
        CloseHandle(port);
        return -2;
    }

    dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 115200
    dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

    Status = SetCommState(port, &dcbSerialParams);  //Configuring the port according to settings in DCB 

    if(Status == FALSE)
    {
        printf("\n    Error! in Setting DCB Structure");
        CloseHandle(port);
        return -3;
    }
    
    /*------------------------------------ Setting Timeouts --------------------------------------------------*/
    
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if(SetCommTimeouts(port, &timeouts) == FALSE)
    {
        printf("\n\n    Error! in Setting Time Outs");
        CloseHandle(port);
        return -4;
    }

    /*------------------------------------ Setting Receive Mask ----------------------------------------------*/
    
    Status = SetCommMask(port, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

    if (Status == FALSE)
    {
        printf("\n\n    Error! in Setting CommMask");
        CloseHandle(port);
        return -5;
    }
#elif __linux__



#endif
    
    return ret;
}


int serial_Settings(SERIALPORT* port, SERIALPARAMS serialParams, SERIALTIMEOUT timeouts)
{

    int ret = 0;

#ifdef _WIN32
    DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    BOOL Status = GetCommState(port, &dcbSerialParams);      //retreives  the current settings

    if(Status == FALSE)
    {
        printf("\n    Error! in GetCommState()");
        CloseHandle(port);
        return -2;
    }

    dcbSerialParams.BaudRate = CBR_115200;      // Setting BaudRate = 115200
    dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

    Status = SetCommState(port, &dcbSerialParams);  //Configuring the port according to settings in DCB 

    if(Status == FALSE)
    {
        printf("\n    Error! in Setting DCB Structure");
        CloseHandle(port);
        return -3;
    }
    
    /*------------------------------------ Setting Timeouts --------------------------------------------------*/
    
    //COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if(SetCommTimeouts(port, &timeouts) == FALSE)
    {
        printf("\n\n    Error! in Setting Time Outs");
        CloseHandle(port);
        return -4;
    }

#elif __linux__


#endif

    return ret;
}


int serail_write(SERIALPORT* port, char* msg)
{
    int ret = 0;

#ifdef _WIN32
    BOOL Status;
    int i, msgLen = strlen(msg);
    DWORD NoBytesWritten;
    do
    {
        Status = WriteFile(port, msg[i], sizeof(char), &NoBytesWritten, NULL);
    } while(NoBytesWritten < msgLen);

#elif __linux__


#endif
    
    return ret;
}


int serial_Read(SERIALPORT* port, char* msg)
{
    int ret = 0;

#ifdef _WIN32
    char  TempChar;                         // Temperory Character
    DWORD dwEventMask;                      // Event mask to trigger
    char  SerialBuffer[256];                // Buffer Containing Rxed Data
    DWORD NoBytesRead;                      // Bytes read by ReadFile()
    BOOL Status;
	
    Status = WaitCommEvent(port, &dwEventMask, NULL); //Wait for the character to be received

    /*-------------------------- Program will Wait here till a Character is received ------------------------*/				

    if(Status == TRUE)
    {
        int i = 0;
        do
        {
            Status = ReadFile(port, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
            SerialBuffer[i] = TempChar;
            i++;
        }
        while (NoBytesRead > 0);

        for(int j = 0; j < i-1; j++)		// j < i-1 to remove the dupliated last character
            printf("%c", SerialBuffer[j]);

    }

#elif __linux__


#endif

    return ret;
}


int serial_Close(SERIALPORT* port)
{
    int ret = 0;

#ifdef _WIN32
	CloseHandle(port);                 //Closing the Serial Port

#elif __linux__


#endif

    return ret;
}


#ifdef RUNABLE
//  Windows/Linux: gcc -Wall -D RUNABLE SerialClient.c -o .\bin\SerialClient
int main(int argc, char* argv[])
{
    SERIALPORT Serial;
    serial_Open(&Serial, argv[1]);
    serial_Write(&Serial, "Hello world!");
    
    char msg[255];
    serial_Read(&Serial, msg);
    serial_Close(&Serial);

    return 0;
}


#endif