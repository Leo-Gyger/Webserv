#include "Server.hpp"
#include "Request.hpp"
#include "parser_utils.hpp"
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

Server::Server() : port(), bodySize() {}


void Server::launch(const Request &req, int fd) const
{
	std::vector<unsigned char> body;

	Response r(getRoutes(), req, this->bodySize);
	std::string ans = r.getResponse().toString();
	if (send(fd, ans.c_str(), ans.size(), 0) <= 0)
		return;

	if (req.getMethod() != "HEAD")
	{
		body = r.getResponse().getBody();
		send(fd, &body[0], body.size(), 0);
	}
	close(fd);
}

Server::~Server() {
//	std::cout << "Server destructed" << std::endl;
}

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
