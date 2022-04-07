#include "../includes/Socket.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

Socket::Socket(int p, const std::string&	addr) : address(), port(p), response_fd()
{
	std::cout << "Socket constructor" << std::endl;
	int val = 1;
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->address.sin_port = htons(port);
	(void)addr;
	this->addrlen = sizeof(address);
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(this->server_fd, SOL_SOCKET,SO_REUSEADDR,(char *)&val,sizeof(val));
	fcntl(this->server_fd, F_SETFL, O_NONBLOCK);
	if (this->server_fd == -1)
	{
		std::cerr << "Error creating the socket" << std::endl;
		exit(1);
	}
}

void Socket::binding()
{
	if (bind(this->server_fd, (struct sockaddr *) &address, sizeof(address)) ==
		-1)
	{
		std::cerr << "Error binding the socket" << std::endl << this->server_fd;
		exit(1);
	}
}

void Socket::listening(int bl) const
{
	listen(this->server_fd, bl);
	std::cout << "Server is listening on port " << this->port << std::endl;
}

Socket::~Socket() {
	if (this->server_fd != -1 && this->server_fd != 0)
		close(this->server_fd);
	std::cout << "destructed" << std::endl; }

Socket &Socket::operator=(const Socket &obj)
{
	this->address = obj.address;
	this->server_fd = obj.server_fd;
	this->response_fd = obj.response_fd;
	this->port = obj.port;
	this->addrlen = obj.addrlen;

	return (*this);
}

void Socket::setPort(int p) { this->port = p; }
int Socket::getPort() const { return (this->port); }
void Socket::setServerFd(int s) { this->server_fd = s; }
int Socket::getServerFd() const { return this->server_fd; }
