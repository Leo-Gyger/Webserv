#include "Server.hpp"
#include "Socket.hpp"
#include "parser.hpp"
#include <poll.h>

void polling(std::vector<struct pollfd> &fds, size_t size)
{
	int status;
	do {
		status = poll(&fds[0], size, 1);
	} while (status == 0 && fds[0].events != fds[0].revents);
	std::cout << status << std::endl;
}

int loop(std::vector<Socket> &sl)
{
	int l = 0;
	std::vector<struct pollfd> fds(sl.size());
	for (std::vector<Socket>::iterator j = sl.begin(); j != sl.end(); ++j, ++l)
	{
		j->listening(10);
		fds[l].fd = sl[l].getServerFd();
		fds[l].events = POLLIN;
		fds[l].revents = 0;
	}

	while (1)
	{
		polling(fds, sl.size());
		for (size_t i = 0; i != sl.size(); ++i)
		{
			std::cout << "revents: " << (fds[i].revents) << std::endl;
			if (fds[i].revents & POLLIN) sl[i].launch();
		}
	}
}

std::vector<Socket> createSockets(std::vector<Server> &sl)
{
	std::vector<Socket> sc;

	for (std::vector<Server>::iterator i = sl.begin(); i != sl.end(); ++i)
	{
		std::vector<Socket>::iterator j;
		for (j = sc.begin(); j != sc.end(); ++j)
		{
			if (j->getPort() == i->getPort())
			{
				std::cout << "Same port!\n";
				if (i->getServerName().empty())
				{
					if (j->defaultServer != NULL)
					{
						std::cerr << "Multiple servers have the same port\n";
						exit(1);
					}
					j->defaultServer = new Server(*i);
				} else
				{
					for (std::vector<Server>::iterator k =
							 j->serverList.begin();
						 k != j->serverList.end(); ++k)
					{
						if (k->getServerName() == i->getServerName())
						{
							std::cerr << "Multiple servers have the same "
										 "server_name and the same port\n";
							exit(1);
						}
					}
					j->serverList.push_back(*i);
				}
				break;
			}
		}
		if (j == sc.end()) sc.push_back(Socket(*i));
	}

	for (std::vector<Socket>::iterator i = sc.begin(); i != sc.end(); ++i)
	{
		if (i->defaultServer == NULL)
		{
			std::cerr << "Each port need a default server\n";
			exit(1);
		}
	}

	return sc;
}

int main(int argc, char **argv)
{

	if (argc != 2) exit(EXIT_FAILURE);
	std::vector<Server> serverList = parse_config_file(argv[1]);

	std::vector<Socket> socketList = createSockets(serverList);
	loop(socketList);
	return 0;
}
