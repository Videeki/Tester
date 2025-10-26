#include "serialClient.h"


int serial_Open(SERIALPORT* port, char* portName)
{
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
    char ComPortName[15] = "/dev/";
    strcat(ComPortName, portName);
    int serial_port = open(ComPortName, O_RDWR);

    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }

#endif
    
    return EXIT_SUCCESS;
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
    struct termios tty;

    // Read in existing settings, and handle any error
    if(tcgetattr(port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }
    tty.c_ispeed = serialParams.BaudRate;
    tty.c_ospeed = serialParams.BaudRate;

    //serialParams.ByteSize;
    //serialParams.Parity;
    switch (serialParams.StopBits)
    {
    case ONE:
        /* code */
        break;
    
    default:
        break;
    }


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


// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int main() {
  // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
  int serial_port = open("/dev/ttyUSB0", O_RDWR);

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  // Write to serial port
  unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
  write(serial_port, msg, sizeof(msg));

  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME
  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

  // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
  if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      return 1;
  }

  // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
  // print it to the screen like this!)
  printf("Read %i bytes. Received message: %s", num_bytes, read_buf);

  close(serial_port);
  return 0; // success
};