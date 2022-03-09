#include "../includes/Server.hpp"
#include <fcntl.h>
#include <vector>
Server::Server(Socket&	s): s(s)
{
	this->s = s;
	std::cout << "Server constructor" << std::endl;
	return;
}

void	Server::launch(void)
{
	std::string te;
	Response	r;
	int	size;
	std::vector	<unsigned char> body;
	std::string ans = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCYPE html>\r\n<html><head><title>Webserv</title></head>\r\n<body><center><h1>Hello World!</h1><br>\r\n<img src=\"test.webp\"</center></body></html>\r\n"; 
	char buf[30000] = {0};
	this->fd = accept(s.server_fd, (struct sockaddr *)&s.address, (socklen_t*)&s.addrlen);
	recv(this->fd, buf, 30000, 0);
	te = buf;
	//std::cout << std::endl << te << std::endl;
	ans = r.answer(te);
	send(this->fd,ans.c_str(),ans.size(), 0);
	body = r.get_body();
	size = r.get_size();
	send(this->fd,(char *)&body[0], size, 0);
	close(this->fd);
}

Server::~Server(void)
{
	std::cout << "destructed" << std::endl;
	return;
}
