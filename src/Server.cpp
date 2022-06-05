#include "Server.hpp"
#include "Request.hpp"
#include "parser_utils.hpp"
#include <poll.h>
#include <vector>
std::map<int, Socket *> Server::smap;
Server::Server() : port(), fd(), bodySize() {}
void npolling(struct pollfd *fds, int size)
{
	int status;
	fds->revents = 0;
	do {
		status = poll(fds, size, 1);
	} while (status == 0 && fds->events != fds->revents);
}
std::string Server::readSocket() const
{
	std::string te;
	struct pollfd fds;
	fds.fd = this->fd;
	fds.events = POLLIN;
	npolling(&fds, 1);
	char *buf = new char[this->bodySize];
	if (!buf)
	{
		std::exit(1);
	}
	for (int in = 0; in != this->bodySize; in++) buf[in] = 0;
	int size = recv(this->fd, buf, this->bodySize, 0);
	if (size <= 0)
	{
		delete[] buf;
		return (std::string());
	}
	te.resize(size);
	te = buf;
	delete[] buf;
	return (te);
}

void leave(int sig)
{
	(void) sig;
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
	std::vector<unsigned char> body;
	std::string ans;
	if (this->fd == -1)
		return;
	std::string buff = readSocket();
	std::cout << buff << std::endl;
	Request req(buff, this->serverName, this->port);
	Response r(getRoutes(), req, this->bodySize);
	ans = r.getResponse().toString();
	std::cout << "ans "	<< ans << std::endl;
	if (send(this->fd, ans.c_str(), ans.size(), 0) <= 0)
	{
		std::cout << "send failed" << std::endl;
		return;
	}

	if (req.getMethod() != "HEAD")
	{
		body = r.getResponse().getBody();
		std::cout << "Send value: " << send(this->fd, &body[0], body.size(), 0) << '\n';
	}
	close(this->fd);
}

Server::~Server()
{
	std::cout << "Server destructed" << std::endl;
}

int Server::getFd() const { return (this->s->getServerFd()); }

void Server::createSocket(const std::string &addr)
{
	static std::vector<std::string> sn_list;
	if (smap.count(this->port) == 0)
	{
		smap.insert(std::make_pair(this->port, new Socket (this->port, addr)));
		sn_list.push_back(this->serverName);
	}
	else 
	{
		for (size_t i = 0; i != sn_list.size(); ++i)
		{
			if (sn_list[i] == this->serverName)
			{
				std::cerr << "can't have two servers with the same server name on the same port" << std::endl;
				exit(1);
			}
		}
	}
	s = smap[this->port];
	this->s->binding();
}
void Server::listen() { this->s->listening(10); }

void Server::setPort(const int &p) { this->port = p; }
int Server::getPort() const { return (this->port); }

void Server::setBody(const int &b) { this->bodySize = b; }
int Server::getBody() const { return this->bodySize; }

void Server::addRoute(const Route &r) { routesList.push_back(r); }
std::vector<Route> Server::getRoutes() const { return this->routesList; }

void Server::setServerName(const std::string &str) { this->serverName = str; }
const std::string &Server::getServerName() const { return this->serverName; }
