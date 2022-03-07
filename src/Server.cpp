#include "../includes/Server.hpp"

Server::Server(Socket&	s): s(s)
{
	this->s = s;
	std::cout << "Server constructor" << std::endl;
	return;
}

void	Server::launch(std::string msg[2])
{
	int	i, a;
	i = 1;
	a = 0;
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
		std::cout << std::endl << std::string(buf) << std::endl;
		write(this->fd, msg[a].c_str(), msg[a].size());
		close(this->fd);
		a++;
	}
}

Server::~Server(void)
{
	std::cout << "destructed" << std::endl;
	return;
}
