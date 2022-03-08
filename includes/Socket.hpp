#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

class Socket
{
public:
	int server_fd;
	int response_fd;
	struct sockaddr_in address;
	unsigned int addrlen;
	Socket(int port);
	~Socket(void);
	void binding(void);
	void listening(int bl);
	void communicate(std::string msg);

private:
	int port;
};
#endif
