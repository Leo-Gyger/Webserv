#include "BindingSocket.hpp"

BindingSocket::BindingSocket(int dom, int serv, int prot, int port, unsigned long interf) : Socket(dom, serv, prot, port, interf)
{
	int o = 1;
	int a;
	std::cout << "BindingSocket constructor" << std::endl;
	this->connection = net_connect(this->server_fd, this->addr);
	a = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,&o, sizeof(int));
	if (a == -1)
		std::cout << "Error setsock" << std::endl;
	std::cout << "Binding the socket" << std::endl;
	test_con(connection);
	return;
}

int	BindingSocket::net_connect(int fd, struct sockaddr_in addr)
{
	return bind(fd,(const struct sockaddr *)&addr, sizeof(addr));
}
