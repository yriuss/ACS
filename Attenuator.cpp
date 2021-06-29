#include "Attenuator.h"

void Attenuator::init() {
    using namespace std;
    serialPorts2scan.insert("/dev/ttyACM0");
    serialPorts2scan.insert("/dev/ttyACM1");
    serialPorts2scan.insert("/dev/ttyACM2");
    serialPorts2scan.insert("/dev/ttyACM3");
    serialPorts2scan.insert("/dev/ttyUSB0");
    serialPorts2scan.insert("/dev/ttyUSB1");
    serialPorts2scan.insert("/dev/ttyUSB2");
    baudrate = 115200;
}

Attenuator::Attenuator(const char* serialNumber) {
    this->serialNumber = serialNumber;
	init();

}

Attenuator& Attenuator::connect() {

    using namespace std;
    serialPort = "";
    for (set<string>::iterator it = serialPorts2scan.begin(); it != serialPorts2scan.end(); ++it) {
        //cout << "Tantando conectar em " << (*it) << endl;
        string SN = getSN(it->c_str(), baudrate);
        cout << "SN=" << SN << "-" << serialNumber << endl;
        if (serialNumber.compare(SN) == 0) {
            serialPort = (*it);
            break;
        }
    }


    serial.sopen(serialPort.c_str(), baudrate);
    return *this;
}

Attenuator& Attenuator::unconnect() {
    if (serial.isOpen())
        serial.sclose();

    return *this;
}

bool Attenuator::isConnected() {
    return serial.isOpen();
}

std::string Attenuator::getSerialPort() {
    if (serial.isOpen())
        return serialPort;

    return "";
}

Attenuator& Attenuator::AttenuatePot(const float powerdB) {
    if (serial.isOpen()) {
        using namespace std;

        string cmd = ("SAA ");
        cmd = cmd + to_string(powerdB);
        serial.swrite(cmd.c_str());
    }

    return *this;
}

std::string getSN(const char* serialPort, unsigned int baudrate) {
    using namespace std;
    TSerial serial;//(serialPort,baudrate);
    
    serial.sopen(serialPort);
    serial.sflush();
    
    string SN = "";

    if (serial.isOpen()) {
        msleep(3500);
        serial.swrite("INFO\n");
        msleep(50);
        stringstream ss;

        while (serial.sread() > 0) {
            ss << serial.read_buf;
        }

        string msg = ss.str();
        //cout << msg << endl;
        std::size_t found = msg.find("SN:");

        if (found != std::string::npos) {
            SN = msg.substr(found + 4, 11);
        }
    }

    return SN;
}

Attenuator::~Attenuator() {
    if (serial.isOpen())
        serial.sclose();
}
