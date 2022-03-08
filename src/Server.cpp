#include "../includes/Server.hpp"

Server::Server(Socket &s) : s(s)
{
	this->s = s;
	std::cout << "Server constructor" << std::endl;
	return;
}

void Server::launch(std::string msg)
{
	while (1)
	{
		char buf[30000] = {0};
		this->fd = accept(s.server_fd, (struct sockaddr *) &s.address,
						  (socklen_t *) &s.addrlen);
		read(this->fd, buf, 30000);
		std::cout << std::endl << std::string(buf) << std::endl;
		write(this->fd, msg.c_str(), msg.size());
		close(this->fd);
	}
}

Server::~Server(void)
{
	std::cout << "destructed" << std::endl;
	return;
}

Server::Server(const Server &s) : s(s.s) {}

Server &Server::operator=(const Server &obj)
{
	this->s = obj.s;
	this->fd = obj.fd;
	return (*this);
}
