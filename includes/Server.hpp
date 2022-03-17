#ifndef SERVER_HPP
#define SERVER_HPP

#include "Response.hpp"
#include "Route.hpp"
#include "Socket.hpp"
#include <fstream>
#include <iostream>
#include <vector>

class Server
{
public:
	Server();
	~Server();

	void launch();

	void createSocket();

	void setPort(int p);
	int getPort() const;

	void setBody(int b);
	int getBody() const;

	void addRoute(const Route &r);
	std::vector<Route> getRoutes();

private:
	int port;
	int fd;
	int bodySize;
	class Socket *s;
	std::vector<Route> routesList;
};

#endif
