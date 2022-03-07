#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#define PORT 8080

class	Socket
{
	public:
		int	server_fd;
		int	response_fd;
		struct sockaddr_in address;
		unsigned int	addrlen;
		Socket(int port)
		{
			memset(&address, 0, sizeof(address));
			memset(address.sin_zero,0, sizeof(address.sin_zero));
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons(port);
			addrlen = sizeof(address);
			server_fd = socket(AF_INET, SOCK_STREAM, 0);
		}
		void	binding(void)
		{
			bind(this->server_fd, (struct sockaddr *)&address,sizeof(address));
		}
		void	listenening(int bl)
		{
			listen(this->server_fd, bl);
		}
		void	communicate(std::string msg)
		{
			while (1)
			{
				char buf[30000] = {0};
				response_fd = accept(this->server_fd, (struct sockaddr *)&this->address, (socklen_t*)&addrlen);
				read(response_fd, buf, 30000);
				std::cout << std::endl << std::string(buf) << std::endl;
				write(response_fd, msg.c_str(), msg.size());
				close (response_fd);
			}
		}
};

int main(void)
{
	Socket a(8080);
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Only this line has been changed. Everything is same.
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    // Creating socket file descriptor 
    a.binding();
    a.listenening(10);
    a.communicate(hello);
    return 0;
}
