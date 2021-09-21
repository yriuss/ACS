#include  <string>
#include "Attenuator.h"
//#define NDEBUG
#include <assert.h>
#include "ToolBoxAndUtilities.h"
#include "SystemTests.h"
#include <fstream>
#include <vector>
#include <map>
#include "TTable.h"
#include "TCommand.h"
#include "TAcs.h"
#include "Attenuator.h"
#include <memory>
//#include <pigpio.h>



typedef char server_arr[14];

//#define  SERVER_ADDR  (server_arr*) "127.0.0.1"//"192.168.0.221" //endereço da comunicaçao entre o tcpserver e o com master
//#define  SERVER_ADDR1 (server_arr*) "192.168.1.101"///"192.168.1.101" //endereço entre tcpclient e o outstation
//#define  SERVER_ADDR2 (server_arr*) "192.168.26.124"
//#define  SERVER_ADDR3 (server_arr*) "192.168.26.124"

char SERVER_ADDR[10] = "127.0.0.1";
char SERVER_ADDR1[14] = "192.168.1.102";
char SERVER_ADDR2[15] = "192.168.26.124";
char SERVER_ADDR3[15] = "192.168.26.124";

std::string GetIPAddress(int DNP3Address)
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


int main(int argc, char** argv) {

    using namespace std;

	TAcs acs;
	//Attenuator att("R3160950386");
	//acs.parseCmd(argc,argv);
	//acs.connectAttenuators();
	//acs.printTest();
	//acs.AttenuatePot();
	
	//att.connect();
	//att.AttenuatePot(50);
	//acs.setPhases();
	//acs.printTest();
	

	int DNP3Address = 1;
	int i;
	pthread_t t1;
		if (acs.connectToCOI(SERVER_ADDR,20000))
		{
			acs.connectToOutstation (SERVER_ADDR1,20000);
			while(1){
				if (acs.read_dnp_msg ())
				{
					
					//DNP3Address = acs.getDNP3Address ();
					
					if (DNP3Address==0)
					{
						printf("nothing");
						break;
					}
					acs.talkToOutstation(SERVER_ADDR,20002,t1);
				}

				else {
					break;
				}
			} 
		
		}
		acs.closeConnection();
	//acs.parseCmd("cmd -pi 1 2 3 4 5 6 7");
    //testTCommand();
    //testTTablet();
    //testToolBoxAndUtilities();
    //testTSerial();
    //testGetSN();
    //testAttenuator();
	
	
    return 0;
}

    