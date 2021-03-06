/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TTAcs.cpp
 * Author: daniel
 * 
 * Created on 17 de Março de 2021, 15:30
 */

#include "TAcs.h"




TAcs::TAcs() {
#ifdef ARCH_RASP
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialisation failed\n");
        exit(1);
    }

    /* Set GPIO modes */
    gpioSetMode(4, PI_OUTPUT);
    gpioSetMode(17, PI_OUTPUT);
    gpioSetMode(27, PI_OUTPUT);
    gpioSetMode(22, PI_INPUT);
    gpioSetMode(10, PI_OUTPUT);
    gpioSetMode(9, PI_OUTPUT);
    gpioSetMode(11, PI_OUTPUT);
    gpioSetMode(0, PI_OUTPUT);

    gpioSetMode(5, PI_OUTPUT);
    gpioSetMode(6, PI_OUTPUT);
    gpioSetMode(13, PI_OUTPUT);
    gpioSetMode(19, PI_INPUT);
    gpioSetMode(26, PI_OUTPUT);
    gpioSetMode(21, PI_OUTPUT);
    gpioSetMode(20, PI_OUTPUT);
    gpioSetMode(16, PI_OUTPUT);

    gpioSetMode(12, PI_OUTPUT);
    gpioSetMode(1, PI_OUTPUT);
    gpioSetMode(7, PI_OUTPUT);
    gpioSetMode(8, PI_INPUT);
    gpioSetMode(25, PI_OUTPUT);
    gpioSetMode(24, PI_OUTPUT);
    gpioSetMode(23, PI_OUTPUT);
    gpioSetMode(18, PI_OUTPUT);
#endif
}


/* This method sets a socket connection to COI*/
int TAcs::connectToCOI(const char* COIAddr, int COIPort)
{
		
		if (!server.createSocket(COIAddr, COIPort))
		{
			return 0;
		}	
			
		if (!server.Bind()){
			return 0;
		}
		
		if (!server.Listen()){
			return 0;
		}
		
		if (!server.Accept())
		{
			return 0;
		}
		
		fprintf(stdout, "Client connected.\nWaiting for client message ...\n");
		
		return 1;
}

int TAcs::read_dnp_frame ()
{
	memset(DNP3Frame, 0x0, LEN);

	if((DNP3FrameLen = server.Read(DNP3Frame, LEN)) <= 0) 
	{
		return 0;  
	}
	return 1;
}

/* Calculate msg len from DNP3Msg */
void TAcs::calculateMsgLen(){
	unsigned short int msg_idx = 10, aux = 0;
	DNP3MsgLen = 5;
	for(int i = 10; i < DNP3FrameLen; i++){
		if((DNP3FrameLen - i) < 3){
			break;
		}
		if(aux == 16){
			DNP3Msg[msg_idx] = DNP3Frame[i];
			i += 1;
			aux = 0;
			continue;
		}
		DNP3Msg[msg_idx] = DNP3Frame[i];
		DNP3MsgLen++;
		msg_idx++;
		aux++;
	}
}

void gen_dnp_crc16(data_arr& data, size_t size) {     
	uint16_t crc = 0, data_size = size;
	int i = 0;
	while (size--) {
		crc ^= data[i++];
		for (unsigned k = 0; k < 8; k++)
			crc = crc & 1 ? (crc >> 1) ^ CRC16_DNP : crc >> 1;
	}
	crc^=0xFFFF;
	data[data_size] = LSBy(crc);
	data[data_size + 1] = MSBy(crc);
}

void TAcs::gen_header(){
	unsigned char data[16] = {0x05, 0x64, DNP3MsgLen, DNP3Frame[3], 0x0a, 0x00, 0x01, 0x00};

	gen_dnp_crc16(data, 8);
	for(int i = 0; i < 10; i++)
		DNP3Msg[i] = data[i];

#ifdef OPEN_DNP3_LIB
	printf("\n\n\n\n");
	printf("Mensagem gerada pelo COI:\n");
	for(int i = 0; i < DNP3FrameLen; i++)
		printf("%02x ", DNP3Frame[i]&0xFF);

	printf("\nMensagem após passar pelo CAT:\n");
	for(int i = 0; i < DNP3FrameLen; i++)
		printf("%02x ", DNP3Msg[i]&0xFF);
	printf("\n");

	
	//printf("\n\n\n\n");

	for(int i = 0; i < DNP3FrameLen; i++)
		DNP3Msg[i] = DNP3Frame[i];

	//printf("%d\n", DNP3MsgLen);
	DNP3MsgLen = DNP3FrameLen;
#else
#endif
}

int TAcs::read_dnp_msg ()
{

#ifdef OPEN_DNP3_LIB
	read_dnp_frame();
	calculateMsgLen();
#else
	memset(DNP3Msg, 0x0, LEN);

	if((DNP3MsgLen = server.Read(DNP3Msg, LEN)) <= 0) 
	{
		return 0;  
	}
#endif

	gen_header();
	/*
	printf("\n\n");	
	for (i = 0; i< DNP3MsgLen; i++)
	{
		printf("%d =%x --",i,DNP3Msg[i]);
	} 
	printf("\n\n");
	*/
	int i;
	/*
	printf("\n\n");
	std::cout << "Read message: " << std::endl;
	for (i = 0; i < DNP3MsgLen - 1; i++)
	{
		printf("%x ", DNP3Msg[i]&0xFF);
	}
	printf("%x\n", DNP3Msg[i]&0xFF);
	printf("\n\n");	*/		

	return 1;
}

int TAcs::getDNP3Address () 
{	
	/**
		* This method extracts the DNP3 address from a DNP3 frame
		* @return DNP3Address
		* @author Daniel L. S. Nascimento
		* @since  14-05-2020
	*/
	int byte1, byte2;
	if(DNP3Msg[4] >= 0xffffff00)
	{
		byte1 = (int) DNP3Msg[4] - 0xffffff00;		
	}
	else
	{
		byte1= (int) DNP3Msg[4]; 
	}
	
	if(DNP3Msg[5] >= 0xffffff00)
	{
		byte2= (int) DNP3Msg[5] - 0xffffff00;			
	}
	else
	{
		byte2= (int) DNP3Msg[5]; 
	}
	
	DNP3Addr = (byte2*0x100) + byte1;
	std::cout<<DNP3Addr<<std::endl;
	
	return (byte2*0x100) + byte1;	
}
	
int TAcs::connectToOutstation (char* OutstationAddr,int port){
	std::cout << "IP Address is " << OutstationAddr << std::endl;
	//std::unique_ptr<TTcpIPClient> client(new TTcpIPClient); Removi pra testar o client dentro da classe
	
	
	if (!client.createSocket(OutstationAddr, port))
	{
		return 0;
	}

	if (!client.Connect()){
		return 0;
	}
	return 1;
}


int TAcs::talkToOutstation (char* OutstationAddr,int port, pthread_t t1)
{
		/*
	printf("\n\n");
	std::cout << "Wrote message: " << std::endl;
	int i;
	for (i = 0; i < DNP3MsgLen - 1; i++)
	{
		printf("%x ", DNP3Msg[i]&0xFF);
	}
	printf("%x\n", DNP3Msg[i]&0xFF);
	printf("\n\n");
	*/
	client.Write(DNP3Msg, DNP3MsgLen);
	do{
		DNP3MsgLen = client.Read(DNP3Msg, LEN, t1);
		/*
		printf("\n\n");
		std::cout << "Read message from outstation: " << std::endl;
		for (i = 0; i < DNP3MsgLen - 1; i++)
		{
			printf("%x ", DNP3Msg[i]&0xFF);
		}
		printf("%x\n", DNP3Msg[i]&0xFF);
		printf("\n\n");*/
		if(DNP3MsgLen>0){
			server.Write(DNP3Msg,DNP3MsgLen);
		}
		printf("\nloading...\n");
	}while(DNP3MsgLen>0);

	signal(SIGCHLD,SIG_IGN);
	
	if (DNP3MsgLen==0)
	{
		return 0;		
	}

	return 1;
}

/* This method closes the CAT connections	*/
void TAcs::closeConnection()
{	
	client.closeSocket();
	server.closeSocket();

}
	

void TAcs::setPhases() {
	float ph_test[3];
	getPhases(ph_test);
	/*
	if(this->isInt()){
		for(int i = 0; i < 3; i++){
			this->ps[i].setPhaseInt(ph_test[i]);
		}
	}
	else{
		for(int i = 0; i < 3; i++)
			this->ps[i].setPhaseDeg(ph_test[i]);
	}
	writePhases();*/
}

void TAcs::connectAttenuators(){
	this->att0.connect();
	if(this->att0.isConnected()) 
        	std::cout << "conexão ok! " << att0.getSerialPort() << std::endl;
	
	this->att1.connect();
	if(this->att1.isConnected()) 
        	std::cout << "conexão ok! " << att1.getSerialPort() << std::endl;
	
	this->att2.connect();
	if(this->att2.isConnected()) 
        	std::cout << "conexão ok! " << att2.getSerialPort() << std::endl;
	
	this->att3.connect();
	if(this->att3.isConnected()) 
        	std::cout << "conexão ok! " << att3.getSerialPort() << std::endl;
}

void TAcs::AttenuatePot(){
	float pot[4];
	this->getAttenuations(pot);
	std::cout << pot[1] << std::endl;
	/*this->att0.AttenuatePot(pot[0]);
	this->att1.AttenuatePot(pot[1]);
	this->att2.AttenuatePot(pot[2]);
	this->att3.AttenuatePot(pot[3]);*/
}



void TAcs::unconnectAttenuators(){
	this->att0.unconnect();
	this->att1.unconnect();
	this->att2.unconnect();
	this->att3.unconnect();
}


TAcs::TAcs(const TAcs& orig) {
	
}


TAcs::~TAcs() {
#ifdef ARCH_RASP
    /* Stop DMA, release resources */
    gpioTerminate();
#endif
}

void TAcs::interceptPacket(int argc, char** argv){
	// IPv4 address of the interface we want to sniff
	std::string interfaceIPAddr = "192.168.10.15";

	// find the interface by IP address
	pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr.c_str());
	if (dev == NULL)
	{
		printf("Cannot find interface with IPv4 address of '%s'\n", interfaceIPAddr.c_str());
		exit(1);
	}
	

	

	// Get device info
	// ~~~~~~~~~~~~~~~

	// before capturing packets let's print some info about this interface
	printf("Interface info:\n");
	// get interface name
	printf("   Interface name:        %s\n", dev->getName().c_str());
	// get interface description
	printf("   Interface description: %s\n", dev->getDesc().c_str());
	// get interface MAC address
	printf("   MAC address:           %s\n", dev->getMacAddress().toString().c_str());
	// get default gateway for interface
	printf("   Default gateway:       %s\n", dev->getDefaultGateway().toString().c_str());
	// get interface MTU
	printf("   Interface MTU:         %d\n", dev->getMtu());
	// get DNS server if defined for this interface
	if (dev->getDnsServers().size() > 0)
		printf("   DNS server:            %s\n", dev->getDnsServers().at(0).toString().c_str());

	// open the device before start capturing/sending packets
	if (!dev->open())
	{
		printf("Cannot open device\n");
		exit(1);
	}

	pcpp::IPFilter ipFilter("192.168.1.20", pcpp::SRC_OR_DST);

	// create a filter instance to capture only TCP traffic
	pcpp::ProtoFilter protocolFilter(pcpp::ICMP);

	// create an AND filter to combine both filters - capture only TCP traffic on port 80
	pcpp::AndFilter andFilter;
	andFilter.addFilter(&ipFilter);
	andFilter.addFilter(&protocolFilter);

	// set the filter on the device
	dev->setFilter(andFilter);

	// create the stats object
	PacketStats stats;


	// Async packet capture with a callback function
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	printf("\nStarting async capture...\n");

	

	// start capture in async mode. Give a callback function to call to whenever a packet is captured and the stats object as the cookie
	dev->startCapture(onPacketArrives, &stats);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	std::string str;
	this->parseCmd(argc, argv);
	// stop capturing packets
	while(1){
			str = stats.cb_packet.toString();
			switch(this->manual){
				case 1:
					if(!str.empty() && stats.icmpPacketCount_cb){
						std::cout << str << std::endl;
						if(!str.substr(str.find("192.168.10.10"), 14).compare("192.168.10.103")){
							std::cout << "ip atual eh: " << "192.168.10.103" << std::endl;
							if(stats.icmpPacketCount_cb > 100)
								break;
						}
						if(!str.substr(str.find("192.168.10.10"), 14).compare("192.168.10.102")){
							std::cout << "ip atual eh: " << "192.168.10.102" << std::endl;
							if(stats.icmpPacketCount_cb > 100)
								break;
						}
					}
				case 0:
					this->parseCmd(argc, argv);
				default:
					break;

			}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	//pcpp::multiPlatformSleep(120); 
	dev->stopCapture();
	std::cout << "Time difference = " << std::dec << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

	printf("\nResults:\n");
	stats.printToConsole();

	// clear stats
	stats.clear();

	// close the device before application ends
	dev->close();
}
static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
	// extract the stats object form the cookie
	PacketStats* stats = (PacketStats*)cookie;

	// parsed the raw packet
	pcpp::Packet parsedPacket(packet);

	stats->cb_rawpacket = packet;
	stats->cb_packet = parsedPacket;
	// collect stats from packet
	stats->consumePacket(parsedPacket);
	
}