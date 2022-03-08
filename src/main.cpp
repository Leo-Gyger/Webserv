#include <Server.hpp>
#include <constants.hpp>
#include <iostream>
#include <parser.hpp>

int main(int argc, char **argv)
{
	if (argc != 2)
		exit(EXIT_FAILURE);
	std::vector<Server> serverList = parse_config_file(argv[1]);

	std::string hello = "HTTP/1.1 200 OK\nContent-Type: "
						"text/plain\nContent-Length: 12\n\nHello world!";

	Server sv(serverList[0]);

	Socket a;
	a.setPort(sv.getPort());
	a.binding();
	a.listening(10);
	sv.setSocket(a);
	sv.launch(hello);

	return 0;
}
