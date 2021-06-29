#include "Serial.h"

void TSerial::initSerialPort(void) {

        // Create new termios struc, we call it 'tty' for convention
        //Control flags
        this->tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        this->tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        this->tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
        this->tty.c_cflag |= CS8; // 8 bits per byte (most common)
        this->tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        this->tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

        this->tty.c_lflag &= ~ICANON;
        this->tty.c_lflag &= ~ECHO; // Disable echo
        this->tty.c_lflag &= ~ECHOE; // Disable erasure
        this->tty.c_lflag &= ~ECHONL; // Disable new-line echo
        this->tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

        //Input flags
        this->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        this->tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

        //Output flags
        this->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        this->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        this->tty.c_cc[VTIME] = 0; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        this->tty.c_cc[VMIN] = 0;
}

    TSerial::TSerial(){
        
    };
    
    TSerial::TSerial(const char* port_path, unsigned int baudrate) {
        sopen(port_path,baudrate);
    };

TSerial& TSerial::sopen(const char* port_path, unsigned int baudrate) {
    using namespace std;
    
    serial_port = open(port_path, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
    //serial_port = open(port_path, O_RDWR | O_NOCTTY);
    cout << "serial_port=" << serial_port << " " << port_path << endl;
    if (serial_port == -1) {
        //throw std::runtime_error(funcName+": Serial port with file descriptor " +
                //std::string(port_path) + " is already locked by another process.");
        cout << __func__ << ": Error, " << port_path << " is already locked by another process." << endl;
    } else
        if (serial_port > 0) {
        
            msleep(100);
            
        tcflush(serial_port, TCIOFLUSH);            
        
        // Read in existing settings, and handle any error
        if (tcgetattr(serial_port, &tty) != 0) {
            std::cout << __func__ << ": Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
        }
            
        initSerialPort();

        // Set in/out baud rate to be 115200
        cfsetispeed(&tty, baudrate);
        cfsetospeed(&tty, baudrate);

        // Save tty settings, also checking for error
        if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
            std::cout << __func__ << ": Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
        }
        
    }
    return *this;
}

bool TSerial::isOpen() {
    if(serial_port>0) 
        return true;
    
    return false;
}

TSerial& TSerial::swrite(const char* msg) {
    if (serial_port > 0) {
        short unsigned int msg_size = (strlen(msg) + 1) * sizeof (char);
#ifdef DEBUG_SERIAL_WRITE
        using namespace std;
        cout << "WRITE: " << msg << endl << flush;
#endif
        write(serial_port, msg, msg_size);
        //tcflush(serial_port, TCIOFLUSH);
    }
    return *this;
}

int TSerial::sread() {

    if (serial_port > 0) {
        // Normally you wouldn't do this memset() call, but since we will just receive
        // ASCII data for this example, we'll set everything to 0 so we can
        // call printf() easily.
        memset(read_buf, 0, MaxBufferSize);

        // Read bytes. The behaviour of read() (e.g. does it block?,
        // how long does it block for?) depends on the configuration
        // settings above, specifically VMIN and VTIME
        read(this->serial_port, this->read_buf, MaxBufferSize);
        strSize = strlen(read_buf);

#ifdef DEBUG_SERIAL_READ
        using namespace std;
        cout << "INFO: " << read_buf << " N=" << strSize << endl << flush;
#endif

        /*	printf("\nThe read information is: '%s'\n",this->read_buf);
                if (num_bytes < 0) {
              printf("Error reading: %s", strerror(errno));
            }*/
    }

    return strSize;
}

TSerial& TSerial::sflush() {
    if (serial_port > 0) {
        tcflush(serial_port, TCIOFLUSH);
        memset(read_buf, 0, MaxBufferSize);
        strSize = 0;
    }

    return *this;
}

TSerial& TSerial::sclose() {
    if (serial_port > 0) {
        close(this->serial_port);
        serial_port = 0;
    }

    return *this;
}

std::string TSerial::str() {
    std::string s(read_buf);
    return s;
}

TSerial::~TSerial() {
    if(isOpen())
        sclose();
}
