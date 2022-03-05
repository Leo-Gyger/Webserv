#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
class Socket
{
	protected:
		int	server_fd;
		int	connection;
		struct sockaddr_in addr;
	public:
		Socket(const Socket& t);
		Socket(int	domain, int service, int protocol, int port, unsigned long inter);
		Socket&	operator=(const Socket& t);
		virtual int net_connect(int fd, struct sockaddr_in) = 0;
		void	test_con(int);
		~Socket(void);

};
//std::ostream	&operator<<(std::ostream &ostream, const Socket &d);

#endif

