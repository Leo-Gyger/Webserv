#include "Webserv.hpp"

int	main(int ac, char *av[])
{
	std::cout << "Hello World" << std::endl;
	std::cout << "binding Socket" << std::endl;
	std::cout << "Listening Socket" << std::endl;
	ListeningSocket l(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10);
	std::cout << "finished" << std::endl;
	return (0);
}

