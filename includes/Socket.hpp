#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

class Socket
{
public:
	Socket(int p);

	~Socket();

	Socket &operator=(const Socket &obj);

	void binding();

	void listening(int bl);

	void communicate(std::string msg);

	void setPort(int p);
	int getPort() const;

	void setServerFd(int s);
	int getServerFd() const;


	struct sockaddr_in address;
	unsigned int addrlen;
private:
	int port;
	int server_fd;
	int response_fd;
};

#endif
