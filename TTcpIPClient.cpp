#include "TTcpIPClient.h"
#include<pthread.h>
#include <unistd.h>
#include <signal.h>
#include<string.h>
#include<iostream>
/* Sockets buffers length */

	
int TTcpIPClient::createSocket (char* serverAddr, int port)
{	
	fprintf(stdout, "Starting Client ...\n");
	/*
	* Creates a socket for the client
	*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("Error on client socket creation:");
		return 0;
	}
	fprintf(stdout, "Client socket created with fd: %d\n", sockfd);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int));
	
	/* Defines the connection properties */
	server.sin_family = AF_INET;         
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(serverAddr);
	memset(server.sin_zero, 0x0, 8);
	
	return 1;
}
	
int TTcpIPClient::Connect ()
{
	int len = sizeof(server);
	if (connected = connect(sockfd, (struct sockaddr*) &server, len) == -1)
	{
		perror("Can't connect to server");
	
		close(sockfd);
		return 0;
	}
	return 1;
}
	
void TTcpIPClient::Write(char* buffer, int len)
{
	write(sockfd, buffer, len);
	memset(buffer, 0x0, LEN);
}

int message_len;
int sock;

void* function (void *arg)
{	
	char *buffer = (char *)(arg);
	message_len = 0;
	printf("\nWaiting for the message\n");
	message_len = read(sock,buffer,LEN);
}



int TTcpIPClient::Read(char* buffer, int len, pthread_t t1)
{	
	sock = sockfd;
	memset(buffer, 0x0, LEN);
	message_len = 0;
	int sock = sockfd;
	pthread_create(&t1, NULL, function, (void *)(buffer));
	
	for (int i=0;i<15000;i++)
	{
		if (message_len!=0)
		{
			break;
		}
		usleep(10);
	}
	pthread_cancel(t1);	
	pthread_join(t1,NULL);
	
	return message_len;
}
	
void TTcpIPClient::closeSocket() 
{	
	close(connected); 
	close(sockfd);
}

TTcpIPClient::~TTcpIPClient()
{
	printf("cleaning client\n");
}