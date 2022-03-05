#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

#include "BindingSocket.hpp"

class ListeningSocket : public BindingSocket
{
	private:
		int	backlog;
	public:
		ListeningSocket(int dom, int serv, int prot, int port, unsigned long inter, int bl);
		void	listening(void);
};
#endif

