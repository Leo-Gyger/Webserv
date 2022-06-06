#ifndef SERVER_HPP
#define SERVER_HPP

#include "Response.hpp"
#include "Route.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <csignal>
#include <map>
class Server
{
public:
	Server();
	Server(const Server &);
	~Server();

	void launch(const Request &, int) const;

	void createSocket(const std::string&	addr);

	void setPort(const int &);
	int getPort() const;

	void setServerName(const std::string &);
	const std::string &getServerName() const;

	void setBody(const int &);
	int getBody() const;
	void addRoute(const Route &r);
	std::vector<Route> getRoutes() const;
private:
	int port;
	int bodySize;
	std::string serverName;
	std::vector<Route> routesList;
};

#endif
