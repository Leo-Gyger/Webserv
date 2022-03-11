#include <Server.hpp>
#include <constants.hpp>
#include <iostream>
#include <parser.hpp>

int main(int argc, char **argv)
{
  if (argc != 2)
		exit(EXIT_FAILURE);
	std::vector<Server> serverList = parse_config_file(argv[1]);
 
	//std::string	hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCYPE html>\r\n<html><head><title>Webserv</title>\r\n<style>body { background-color: #FFFF00}</style></head>\r\n<body><center><h1>Hello World!</h1><br></center></body></html>\r\n";
	Socket a(PORT);
	a.binding();
	a.listening(10);
	Server	e(a);
	e.launch();
	return 0;
}