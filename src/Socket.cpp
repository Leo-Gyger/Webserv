#include "../includes/Socket.hpp"

Socket::Socket(int p): port(p)
{
	std::cout << "Socket constructor" << std::endl;
	memset(&this->address, 0, sizeof(this->address));
	memset(&this->address.sin_zero, 0, sizeof(this->address.sin_zero));
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(port);
	this->addrlen = sizeof(address);
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd == -1)
	{
		std::cerr << "Error creating the socket" << std::endl;
		exit (1);
	}

	return;
}

void Socket::binding()
{
	if (bind(this->server_fd, (struct sockaddr *) &address, sizeof(address)) == -1)
	{
		std::cerr << "Error binding the socket" << std::endl;
		exit(1);
	}
}

void Socket::listening(int bl)
{
	listen(this->server_fd, bl);
	std::cout << "Server is listening on port " << this->port << std::endl;
}

Socket::~Socket() { std::cout << "destructed" << std::endl; }

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
