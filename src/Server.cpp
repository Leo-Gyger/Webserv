#include "Server.hpp"
#include "Request.hpp"
#include "parser_utils.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

Server::Server() : port(), bodySize() {}


void Server::launch(const Request &req, int fd) const
{
	std::vector<unsigned char> body;

	Response r(getRoutes(), req, this->bodySize);
	std::string ans = r.getResponse().toString();
	ssize_t i = send(fd, ans.c_str(), ans.size(), 0);
	if (i == 0)
	{
		close(fd);
		return;
	}
	if (i == -1)
	{
		close(fd);
		return;
	}

	if (req.getMethod() != "HEAD")
	{
		body = r.getResponse().getBody();
		i = send(fd, &body[0], body.size(), 0);
		if (i == 0)
		{
			close(fd);
			return;
		}
		if (i == -1)
		{
			close(fd);
			return;
		}
	}
	close(fd);
}

Server::~Server() {}

void Server::setPort(const int &p) { this->port = p; }
int Server::getPort() const { return (this->port); }

void Server::setBody(const int &b) { this->bodySize = b; }
int Server::getBody() const { return this->bodySize; }

void Server::addRoute(const Route &r) { routesList.push_back(r); }
std::vector<Route> Server::getRoutes() const { return this->routesList; }

void Server::setServerName(const std::string &str) { this->serverName = str; }
const std::string &Server::getServerName() const { return this->serverName; }

Server::Server(const Server &sv)
	: port(sv.port), bodySize(sv.bodySize), serverName(sv.serverName),
	  routesList(sv.routesList)

{}
