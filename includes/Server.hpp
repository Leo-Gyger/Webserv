#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include "Socket.hpp"

class	Server
{
	private:
		int	fd;
		class Socket&	s;
	public:
		Server(class Socket&	s);
		~Server(void);
		void	launch(std::string msg[2]);
};
#endif

