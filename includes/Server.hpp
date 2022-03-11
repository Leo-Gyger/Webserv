#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include "Socket.hpp"
#include "Response.hpp"
class	Server
{
	private:
  	int port;
		int	fd;
		class Socket	*s;
	public:
		Server();
		~Server();
		void	launch();
  		void setPort(int p);
		void createSocket();

		int	getPort() const;
};

#endif
