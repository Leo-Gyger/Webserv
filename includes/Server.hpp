#ifndef SERVER_HPP
#define SERVER_HPP

#include "Response.hpp"
#include "Route.hpp"
#include "Socket.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <csignal>
class Server
{
public:
	Server();
	~Server();

	void launch();

	void createSocket(const std::string&	addr);

	void setPort(int p);
	int getPort() const;

	void setBody(int b);
	int getBody() const;
	int	getFd() const;
	void addRoute(const Route &r);
	std::vector<Route> getRoutes() const;
private:
	int port;
	struct pollfd *fds;
	int fd;
	int bodySize;
	//class Socket *s;
	std::vector<class Socket> s;
	std::vector<Route> routesList;

	std::string readSocket() const;
};

#endif
