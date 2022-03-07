#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include "Server.hpp"
int main(void)
{
	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	Socket a(8080); 
	a.binding();
	a.listening(10);
	Server	e(a);
	e.launch(hello);
	return 0;
}
