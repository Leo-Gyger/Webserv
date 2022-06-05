#ifndef SERVER_HPP
#define SERVER_HPP

#include "Response.hpp"
#include "Route.hpp"
#include "Socket.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <csignal>
#include <map>
class Server
{
public:
	Server();
	~Server();

	void launch();
	void accepting();
	void createSocket(const std::string&	addr);

	void setPort(const int &);
	int getPort() const;

	void setServerName(const std::string &);
	const std::string &getServerName() const;

	void setBody(const int &);
	int getBody() const;
	int	getFd() const;
	void	listen();
	void addRoute(const Route &r);
	std::vector<Route> getRoutes() const;
private:
	int port;
	int fd;
	int bodySize;
	std::string serverName;
	class Socket *s;
	std::vector<Route> routesList;
	static std::map<int,Socket *> smap;
	std::string readSocket() const;
};

#endif
