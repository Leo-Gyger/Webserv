#include "../includes/Server.hpp"
#include <fcntl.h>
Server::Server(Socket&	s): s(s)
{
	this->s = s;
	std::cout << "Server constructor" << std::endl;
	return;
}

void	Server::launch(std::string msg)
{
	int	i,a;
	i = 0;
	a = 0;
	std::string te;
	while (1)
	{
		char buf[30000] = {0};
		this->fd = accept(s.server_fd, (struct sockaddr *)&s.address, (socklen_t*)&s.addrlen);
		i = recv(this->fd, buf, 30000, 0);
		if (i == 0)
		{
			std::cout << "finished" << std::endl;
			break;
		}
		i = 0;
		te = buf;
		std::cout << std::endl << te << std::endl;
		send(this->fd, msg.c_str(), msg.size(), 0);
		a++;
		close(this->fd);
	}
}

Server::~Server(void)
{
	std::cout << "destructed" << std::endl;
	return;
}
