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
		class Socket&	s;
	public:
		Server(class Socket&	s);
		~Server(void);
		void	launch(void);
  	void setPort(int p);
	  int getPort() const;
};

#endif
