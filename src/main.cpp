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
	std::string	hello[] = {"HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\nHello world I'm Leo!", "HTTP/1.1 404 Not Found\n"};
	Socket a(PORT);
	a.binding();
	a.listening(10);
	Server	e(a);
	e.launch(hello);
	return 0;
}
