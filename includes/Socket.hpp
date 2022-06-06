#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Server.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>

#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Socket
{
public:
	explicit Socket(const Server& sv);

	~Socket();

	Socket &operator=(const Socket &obj);

	void binding();

	void listening(int bl) const;

	void communicate(std::string msg);

	void setPort(int p);
	int getPort() const;

	int getServerFd() const;

	void launch();

	std::vector<Server> serverList;
	Server *defaultServer;

	struct sockaddr_in address;
	unsigned int addrlen;


private:
	int epfd;
	int port;
	int response_fd;
	int server_fd;
	int fd;

	std::string readSocket() const;
};

#endif
