#include "../includes/Server.hpp"

Server::Server(): port(80)
{
	std::cout << "Server constructor" << std::endl;
}

void Server::launch(std::string msg)
{
	while (1)
	{
		char buf[30000] = {0};
		this->fd = accept(s.getServerFd(), (struct sockaddr *) &s.address,
						  (socklen_t *) &s.addrlen);
		read(this->fd, buf, 30000);
		std::cout << std::endl << std::string(buf) << std::endl;
		write(this->fd, msg.c_str(), msg.size());
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
