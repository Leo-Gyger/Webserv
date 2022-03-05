#include "Socket.hpp"

Socket::Socket(int domain, int service, int port, int protocol, unsigned long inter)
{
	//initialize struct
	addr.sin_family = domain;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(inter);
	//socket
	server_fd = socket(domain, service, protocol);
	test_con(server_fd);
	return;
}

void	Socket::test_con(int tested)
{
	if (tested < 0)
	{
		perror("connection failed");
		exit(EXIT_FAILURE);
	}
}

Socket& Socket::operator=(const Socket& t)
{
	std::cout << "copy assignment" << std::endl;
	this->server_fd = t.server_fd;
	this->connection = t.connection;
	this->addr.sin_family = t.addr.sin_family;
	this->addr.sin_port = t.addr.sin_port;
	this->addr.sin_addr.s_addr = t.addr.sin_addr.s_addr;
	return (*this);
}

Socket::~Socket(void)
{
	std::cout << "destructed" << std::endl;
	return;
}

