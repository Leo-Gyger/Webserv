#include "Server.hpp"
#include "parser.hpp"
#include <poll.h>

int	polling(std::vector<struct pollfd>& fds, int size)
{
	int	status;
	do
	{
		status = poll(&fds[0], size, 1);
	} while (status == 0 && fds[0].events != fds[0].revents);
	std::cout << status << std::endl;
	return (status);
}

int	loop(std::vector<Server>&	sl)
{

	for (size_t i = 0; i != sl.size(); i++)
	{
		sl[i].createSocket("127.0.0.1");
		sl[i].listen();
	}
	std::vector<struct pollfd> fds(sl.size());
	for (size_t i = 0; i != sl.size(); i++)
	{
		fds[i].fd = sl[i].getFd();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}
	while (1)
	{
		polling(fds, sl.size());
		for (size_t i = 0; i != sl.size(); ++i)
		{
			if (fds[i].revents == POLLIN)
			{
				sl[i].accepting();
				sl[i].launch();
			}
		}
	}
	return (0);
}


int main(int argc, char **argv)
{
	
	if (argc != 2) exit(EXIT_FAILURE);
	std::vector<Server> serverList = parse_config_file(argv[1]);

	loop(serverList);
//	static Server sv = serverList[0];
//	sv.createSocket();
//	sv.launch();
	//std::string	hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html>\r\n<html><head><title>Webserv</title>\r\n<style>body { background-color: #FFFF00}</style></head>\r\n<body><center><h1>Hello World!</h1><br></center></body></html>\r\n";
	/*Socket a();
	a.binding();
	a.listening(10);
	Server	e(a);
	e.launch(); */
	return 0;
}
