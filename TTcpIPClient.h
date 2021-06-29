#ifndef TTcpIPClient_H
#define TTcpIPClient_H

/* Sockets buffers length */
#define LEN 4096 //tamanho da string do payload
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

class TTcpIPClient
{	
	protected:
		struct sockaddr_in server;
		int sockfd;
		int connected;
		int port;	
	public:
		int createSocket (char* serverAddr, int port);
		int Connect ();
		void Write(char* buffer, int len);
		int Read(char* buffer, int len, pthread_t t1);
		void closeSocket();
		~TTcpIPClient();
};

#endif
