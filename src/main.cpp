#include "Server.hpp"
#include "parser.hpp"
#include <poll.h>

int	loop(std::vector<Server>&	sl)
{

	for (size_t i = 0; i != sl.size(); i++)
	{
		sl[i].createSocket("127.0.0.1");
		sl[i].listen();
	}
	while (1)
	{
		for (size_t i = 0; i != sl.size(); ++i)
		{
			sl[i].listen();
			sl[i].accepting();
			sl[i].launch();
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
