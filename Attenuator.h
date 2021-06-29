#ifndef Attenuator_H
#define Attenuator_H

#include <string>
#include <sstream>
#include <set>
#include "Serial.h"

//#define DEBUG_ATTENUATOR

enum LogLevel
{
	EXIT_ERROR = 0,
	EXIT_SUCCESSFULLY,
	ATTENUATOR_1,
	ATTENUATOR_2,
	ATTENUATOR_3,
	ATTENUATOR_4
};

class Attenuator
{
private:
	TSerial serial;
	std::string serialNumber;
	std::string serialPort;
        unsigned int baudrate;
        std::set<std::string> serialPorts2scan;
        
        virtual void init();
public:

	Attenuator(const char* serialNumber);
        Attenuator& connect();
        Attenuator& unconnect();
        bool isConnected();
        std::string getSerialPort();
	Attenuator& AttenuatePot(const float powerdB);
	~Attenuator();
};

std::string getSN(const char* serialPort,unsigned int baudrate);


#endif