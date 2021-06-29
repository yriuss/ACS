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


/* endereços */
#define SERVER_ADDR "127.0.0.1"//"192.168.0.221" //endereço da comunicaçao entre o tcpserver e o com master
#define  SERVER_ADDR1 "192.168.1.101" //endereço entre tcpclient e o outstation
#define  SERVER_ADDR2 "192.168.26.124"
#define  SERVER_ADDR3 "192.168.26.124"





DNP3Address ();
					
					if (DNP3Address==0)
					{
						printf("nothing");
						break;
					}
					acs.talkToOutstation("192.168.1.101",20000,t1);
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

    