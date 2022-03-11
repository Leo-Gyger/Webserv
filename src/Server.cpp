#include "../includes/Server.hpp"
#include <fcntl.h>
#include <vector>
Server::Server(Socket& s): s(s)
{
	std::cout << "Server constructor" << std::endl;
}

void	Server::launch(void)
{
	std::string te;
	Response	r;
	int	size;
	std::vector	<unsigned char> body;
	std::string	ans;
	while (1)
	{
		char buf[30000] = {0};
		this->fd = accept(s.server_fd, (struct sockaddr *)&s.address, (socklen_t*)&s.addrlen);
		recv(this->fd, buf, 30000, 0);
		te = buf;
		std::cout << te << std::endl;
		ans = r.answer(te);
		send(this->fd,ans.c_str(),ans.size(), 0);
		body = r.get_body();
		size = r.get_size();
		send(this->fd,(char *)&body[0], size, 0);
		close(this->fd);
	}
}

Server::~Server()
{
	std::cout << "destructed" << std::endl;
}

Server::Server(const Server &server) {
	*this = server;
}

Server &Server::operator=(const Server &server)
{
	this->port = server.port;

	return (*this);
}

void Server::setPort(int p) {
	this->port = p;
}
int Server::getPort() const {
	return (this->port);
}
void Server::setSocket(Socket socket) {
	this->s = socket;
}
Socket Server::getSocket() const {
	return (this->s);
}
