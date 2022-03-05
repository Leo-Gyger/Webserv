#ifndef CONNECTINGSOCKET_HPP
# define CONNECTINGSOCKET_HPP

#include "Socket.hpp"

class ConnectingSocket : public Socket
{
	ConnectingSocket(int dom, int serv, int prot, int port, unsigned long inter);
	int	net_connect(int fd, struct sockaddr_in addr);
};
#endif

