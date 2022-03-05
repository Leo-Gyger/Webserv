#ifndef BINDINGSOCKET_HPP
# define BINDINGSOCKET_HPP

#include "Socket.hpp"
class BindingSocket : public Socket
{
	public:
		BindingSocket(int	domain, int service, int protocol, int port, unsigned long inter);
		int net_connect(int fd, struct sockaddr_in);
};
//std::ostream	&operator<<(std::ostream &ostream, const Socket &d);

#endif

