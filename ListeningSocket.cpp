#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int dom, int serv, int prot, int port, unsigned long inter, int bl) : BindingSocket(dom, serv, prot, port, inter)
{
	std::cout << "ListeningSocket constructor" << std::endl;
	this->backlog = bl;
	listening();
	return;
}

void	ListeningSocket::listening(void)
{
	listen(this->server_fd, this->backlog);
}
