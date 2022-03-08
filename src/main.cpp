#include <Server.hpp>
#include <constants.hpp>
#include <iostream>
#include <netinet/in.h>
#include <parser.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc == 2) parse_config_file(argv[1]);

	std::string hello = "HTTP/1.1 200 OK\nContent-Type: "
						"text/plain\nContent-Length: 12\n\nHello world!";
	Socket a(PORT);
	a.binding();
	a.listening(10);
	Server e(a);
	e.launch(hello);

	return 0;
}
