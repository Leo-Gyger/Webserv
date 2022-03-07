#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

class	Socket
{
	public:
		int	server_fd;
		int	response_fd;
		struct sockaddr_in address;
		unsigned int	addrlen;
		Socket(int port);
		~Socket(void);
		void	binding(void);
		void	listening(int bl);
		void	communicate(std::string msg);
};
#endif

