#ifndef TTcpIPServer_H
#define TTcpIPServer_H

#include<string>
#include<iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

class TTcpIPServer
{	
	protected:
		int serverfd, clientfd;
		struct sockaddr_in client, server;
		int port;
		
	public:
		int createSocket(const char* serverAddr, int port);
		int Bind();
		int Listen ();
		int Accept ();
		int Read (char* buffer, int len);
		int Write (char* buffer, int len);
		void closeSocket ();
		
};

#endif
