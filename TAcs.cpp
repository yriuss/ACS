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

char* GetIPAddress(int DNP3Address)
{

	if (DNP3Address ==10)
		return SERVER_ADDR1;
	
	if (DNP3Address ==5)
		return SERVER_ADDR2;
	
	return SERVER_ADDR3;
	
} 

int GetPort(int DNP3Address)
{
	std::cout << "DNP3 Address is: " << DNP3Address << std::endl;
	if (DNP3Address ==10)
		return 20000;
	
	if (DNP3Address ==3)
		return 20000;
	
	if (DNP3Address ==4)
		return 20000;
	
	return 20000;
	
}


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
int TAcs::connectToCOI(char* COIAddr, int COIPort)
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


int TAcs::readDNP3frame ()
{		
		/**
		* This method receives the DNP3frame from the COI
		* @return True or False
		* @author Daniel L. S. Nascimento
		* @since  14-05-2020
		*/
		memset(DNP3Frame, 0x0, LEN);
	
		if((DNP3FrameLen = server.Read(DNP3Frame, LEN)) <= 0) 
		{
		    return 0;  
	   	 }
	   
	   	 

		int i;
	/*
	printf("\n\n");	
	for (i = 0; i< DNP3FrameLen; i++)
	{
		printf("%d =%x --",i,DNP3Frame[i]);
	} 
	printf("\n\n");
	*/
	printf("\n\n");
	std::cout << "Read message: " << std::endl;
	for (i = 0; i < DNP3FrameLen - 1; i++)
	{
		std::cout << std::hex << (int)DNP3Frame[i] << " ";
	}
	std::cout << std::hex << (int)DNP3Frame[i] << std::endl;
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
	if(DNP3Frame[4] >= 0xffffff00)
	{
		byte1 = (int) DNP3Frame[4] - 0xffffff00;		
	}
	else
	{
		byte1= (int) DNP3Frame[4]; 
	}
	
	if(DNP3Frame[5] >= 0xffffff00)
	{
		byte2= (int) DNP3Frame[5] - 0xffffff00;			
	}
	else
	{
		byte2= (int) DNP3Frame[5]; 
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
	for (i = 0; i < DNP3FrameLen - 1; i++)
	{
		std::cout << std::hex << (int)DNP3Frame[i] << " ";
	}
	std::cout << std::hex << (int)DNP3Frame[i] << std::endl;
	printf("\n\n");
	
	client.Write(DNP3Frame, DNP3FrameLen);
	do{
		DNP3FrameLen = client.Read(DNP3Frame, LEN, t1);
		printf("\n\n");
		std::cout << "Read message from outstation: " << std::endl;
		for (i = 0; i < DNP3FrameLen - 1; i++)
		{
			std::cout << std::hex << (int)DNP3Frame[i] << " ";
		}
		std::cout << std::hex << (int)DNP3Frame[i] << std::endl;
		printf("\n\n");
		if(DNP3FrameLen>0){
			server.Write(DNP3Frame,DNP3FrameLen);
		}
		printf("\n carregando\n");
	}while(DNP3FrameLen>0);

	signal(SIGCHLD,SIG_IGN);
	
	printf ("correct!!\n");
	if (DNP3FrameLen==0)
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