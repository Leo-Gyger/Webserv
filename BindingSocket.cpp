#include "BindingSocket.hpp"

BindingSocket::BindingSocket(int dom, int serv, int prot, int port, unsigned long interf) : Socket(dom, serv, prot, port, interf)
{
	std::cout << "BindingSocket constructor" << std::endl;
	this->connection = net_connect(this->server_fd, this->addr);
	test_con(connection);
	return;
}

int	BindingSocket::net_connect(int fd, struct sockaddr_in addr)
{
	return bind(fd,(const struct sockaddr *)&addr, sizeof(addr));
}
