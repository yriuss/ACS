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
	/**
	* This method receives the DNP3Msg from the COI
	* @return True or False
	* @author Daniel L. S. Nascimento
	* @since  14-05-2020
	*/
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
	for(int i = 0; i < DNP3FrameLen; i++)
		printf("%02x ", DNP3Msg[i]&0xFF);
	printf("\n");
	for(int i = 0; i < DNP3FrameLen; i++)
		printf("%02x ", DNP3Frame[i]&0xFF);
	printf("\n\n\n\n");

	for(int i = 0; i < DNP3FrameLen; i++)
		DNP3Msg[i] = DNP3Frame[i];
	printf("%d\n", DNP3MsgLen);
	DNP3MsgLen = DNP3FrameLen;
#else
#endif
}

int TAcs::read_dnp_msg ()
{		
	/**
	* This method receives the DNP3Msg from the COI
	* @return True or False
	* @author Daniel L. S. Nascimento
	* @since  14-05-2020
	*/

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
	printf("\n\n");
	std::cout << "Read message: " << std::endl;
	for (i = 0; i < DNP3MsgLen - 1; i++)
	{
		printf("%x ", DNP3Msg[i]&0xFF);
	}
	printf("%x\n", DNP3Msg[i]&0xFF);
	printf("\n\n");			

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
		
	printf("\n\n");
	std::cout << "Wrote message: " << std::endl;
	int i;
	for (i = 0; i < DNP3MsgLen - 1; i++)
	{
		printf("%x ", DNP3Msg[i]&0xFF);
	}
	printf("%x\n", DNP3Msg[i]&0xFF);
	printf("\n\n");
	
	client.Write(DNP3Msg, DNP3MsgLen);
	do{
		DNP3MsgLen = client.Read(DNP3Msg, LEN, t1);
		printf("\n\n");
		std::cout << "Read message from outstation: " << std::endl;
		for (i = 0; i < DNP3MsgLen - 1; i++)
		{
			printf("%x ", DNP3Msg[i]&0xFF);
		}
		printf("%x\n", DNP3Msg[i]&0xFF);
		printf("\n\n");
		if(DNP3MsgLen>0){
			server.Write(DNP3Msg,DNP3MsgLen);
		}
		printf("\n carregando\n");
	}while(DNP3MsgLen>0);

	signal(SIGCHLD,SIG_IGN);
	
	printf ("correct!!\n");
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