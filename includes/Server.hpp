#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include <iostream>

class Server
{
public:
	Server();
	Server(const Server &s);
	~Server();
	Server &operator=(const Server &obj);

	void setPort(int p);
	void setSocket(Socket socket);
	int getPort() const;
	Socket getSocket() const;


	void launch(std::string msg);

private:
	int port;
	int fd;
	Socket s;
};

#endif
