#include "Server.hpp"
#include "Request.hpp"
#include "parser_utils.hpp"
#include <poll.h>
#include <vector>

Server::Server() : port(200), fd(), bodySize(3000) {}

std::string Server::readSocket() const
{
	std::string te;

	char *buf = new char[this->bodySize];
	for (int in = 0; in != this->bodySize; in++) buf[in] = 0;
	size_t size = recv(this->fd, buf, this->bodySize, 0);
	if (size == 0)
	{
		close(this->fd);
		delete[] buf;
		exit(1);//todo: we should not exit
	}
	te.resize(size);
	buf[size] = 0;
	te = buf;
	delete[] buf;
	return (te);
}

void	leave(int sig)
{
	(void)sig;
	std::exit(1);
}

void Server::accepting()
{
		this->fd = accept(s->getServerFd(), (struct sockaddr *) &s->address,
					(socklen_t *) &s->addrlen);
}
void Server::launch()
{
	std::string te;
	size_t size;
	std::vector<unsigned char> body;
	std::string ans;
		if (this->fd == -1)
		{
			close(this->fd);
			return ;
		}
		std::string buff = readSocket();
		std::string serverName =
			"localhost"; /* TODO: fix hardcoded serverName */
		Request req(buff, serverName, this->port);
		while (!req.isFull())
		{
			buff = readSocket();
			if (!req.appendBody(buff))
			req = Request(buff, serverName, this->port);
		}
		Response r(getRoutes(), req);
		ans = r.getRequest();
		send(this->fd, ans.c_str(), ans.size(), 0);
		body = r.get_body();
		size = r.get_size();
		send(this->fd, (char *) &body[0], size, 0);
		close(this->fd);
}

Server::~Server() 
{
	close (this->fd);
	std::cout << "Server destructed" << std::endl;
}

int	Server::getFd() const
{
	return (this->s->getServerFd());
}

void Server::createSocket(const std::string&	addr)
{
	this->s = new Socket(this->port,addr);
	this->s->binding();
}
void	Server::listen()
{
	this->s->listening(10);
}

void Server::setPort(int p) { this->port = p; }
int Server::getPort() const { return (this->port); }

void Server::setBody(int b) { this->bodySize = b; }
int Server::getBody() const { return this->bodySize; }

void Server::addRoute(const Route &r) { routesList.push_back(r); }
std::vector<Route> Server::getRoutes() const { return this->routesList; }
