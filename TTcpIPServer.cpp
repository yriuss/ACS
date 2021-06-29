#include "TTcpIPServer.h"

	
int TTcpIPServer::createSocket (char* serverAddr, int port)
{
	fprintf(stdout, "Starting server\n");
/* Creates a IPv4 socket */
	serverfd = socket(AF_INET, SOCK_STREAM, 0); //criando o socket com IPv4(AF_INET) e TCP head(sock_stream)
	
	if(serverfd == -1) 
	{
		perror("Can't create the server socket:");
		return 0;
	}	
	
	fprintf(stdout, "Server socket created with fd: %d\n", serverfd);
	/* Define as propriedades do socket*/
	server.sin_family = AF_INET;
	server.sin_port = htons(port); //numero da porta
	server.sin_addr.s_addr = inet_addr(serverAddr);
	memset(server.sin_zero, 0x0, 8); //limpando string
	
	/* verifica se o socket ja esta sendo utilizado */
	int yes = 1;
	
	if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(int)) == -1) 
		{ //verifica se o socket ja esta sendo utilizado
			perror("Socket options error:");
			return 0;
		}
	return 1;
}
	
int TTcpIPServer::Bind()
{
	//Coloca um endereço local, endereço IP e porta, no socket
	if(bind(serverfd, (struct sockaddr*)&server, sizeof(server)) == -1) 
	{
		perror("Socket bind error:");
		return 0;
	}
	return 1;
}

int TTcpIPServer::Listen ()
{
//Instrui o sistema operacional para colocar o socket em modo passivo
	if(listen(serverfd, 1) == -1)
	{
		perror("Listen error:");
		return 0;
	}
	fprintf(stdout, "Listening on port %d\n", port);	
	return 1;
}

int TTcpIPServer::Accept ()
{
	//Aceita uma nova conexão
	socklen_t client_len = sizeof(client);
	if ((clientfd=accept(serverfd, (struct sockaddr *) &client, &client_len )) == -1) 
	{
		perror("Accept error:");
		return 0;
	}
	return 1;
}

int TTcpIPServer::Read (char* buffer, int len)
{	
	/* limpando a string do payload */
	memset(buffer, 0x0, len);
	int message_len;
	message_len = read(clientfd, buffer, len);		
	return message_len;
}

int TTcpIPServer::Write (char* buffer, int len)
{
	if (write(clientfd, buffer, len)<0)
	{
		return 0;
	}	
	memset(buffer, 0x0, LEN);
	return 1;
}
	
void TTcpIPServer::closeSocket ()
{	
	close(clientfd);
	close(serverfd);
}
