#include "../includes/Server.hpp"
#include <fcntl.h>
#include <vector>
Server::Server(void) { std::cout << "Server constructor" << std::endl; }

void Server::launch(void)
{
	std::string te;
	Response r;
	int size;
	std::vector<unsigned char> body;
	std::string ans;
	while (1)
	{
		char buf[30000] = {0};
		this->fd = accept(s->getServerFd(), (struct sockaddr *) &s->address,
						  (socklen_t *) &s->addrlen);
		if (this->fd == -1)
		{
			close(this->fd);
			exit(1);
		}
		if (recv(this->fd, buf, 30000, 0) == 0)
		{
			close(this->fd);
			exit(1);
		}
		te = buf;
		std::cout << te << std::endl;
		ans = r.answer(te);
		send(this->fd, ans.c_str(), ans.size(), 0);
		body = r.get_body();
		size = r.get_size();
		send(this->fd, (char *) &body[0], size, 0);
		close(this->fd);
	}
}

Server::~Server()
{
	std::cout << "destructed" << std::endl;
}

void Server::createSocket()
{
	this->s = new Socket(this->port);
	this->s->binding();
	this->s->listening(10);
}

void Server::setPort(int p) { this->port = p; }
int Server::getPort() const { return (this->port); }

void Server::addRoute(const Routes& r) { routesList.push_back(r); }
std::vector<Routes> Server::getRoutes() { return this->routesList; }
