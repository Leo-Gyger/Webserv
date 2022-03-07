#include "Socket.hpp"

Socket::Socket(int port)
{
	std::cout << "Socket constructor" << std::endl;
	memset(&this->address, 0, sizeof(this->address));
	memset(&this->address.sin_zero, 0, sizeof(this->address.sin_zero));
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(port);
	this->addrlen = sizeof(address);
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);	
	return;
}
void	Socket::binding(void)
{
	bind(this->server_fd, (struct  sockaddr *)&address, sizeof(address));
}

void	Socket::listening(int	bl)
{
	listen(this->server_fd, bl);
}

Socket::~Socket(void)
{
	std::cout << "destructed" << std::endl;
	return;
}

