#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include <iostream>

class Server
{
public:
	Server(class Socket &s);
	Server(const Server &s);
	Server();
	~Server();
	Server &operator=(const Server &obj);
	void launch(std::string msg);

private:
	int fd;
	class Socket &s;
};

#endif
