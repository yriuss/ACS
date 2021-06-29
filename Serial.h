#ifndef Serial_H
#define Serial_H

#include <string.h>
#include <iostream>
#include <sys/file.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include "ToolBoxAndUtilities.h"

//#define DEBUG_SERIAL_ALL

#ifdef DEBUG_SERIAL_ALL
#define DEBUG_SERIAL_OPEN
#define DEBUG_SERIAL_WRITE
#define DEBUG_SERIAL_READ
#define DEBUG_SERIAL_CLOSE
#define DEBUG_SERIAL_FLUSH
#define DEBUG_SERIAL_BUFFER
#endif


class TSerial {
private:
    int serial_port=0;
    struct termios tty;
    virtual void initSerialPort(void);
public:
    static const unsigned short int MaxBufferSize = 255;
    short unsigned int strSize;
    char read_buf[MaxBufferSize];

    TSerial();
    TSerial(const char* port_path, unsigned int baudrate = B115200);
    TSerial& sopen(const char* port_path, unsigned int baudrate = B115200);
    bool isOpen();
    TSerial& swrite(const char* msg);
    int sread();
    TSerial& sflush();
    TSerial& sclose();
    std::string str();
    ~TSerial();
};

#endif
