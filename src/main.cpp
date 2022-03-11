#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <Server.hpp>
#include <constants.h>

int main(void)
{
	//std::string	hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCYPE html>\r\n<html><head><title>Webserv</title>\r\n<style>body { background-color: #FFFF00}</style></head>\r\n<body><center><h1>Hello World!</h1><br></center></body></html>\r\n";
	Socket a(PORT);
	a.binding();
	a.listening(10);
	Server	e(a);
	e.launch();
	return 0;
}
