#include "Response.hpp"
#include <fstream>
Response::Response(void)
{
	return;
}

std::string	Response::answer(std::string& msg)
{
	std::string::size_type i;

	i = msg.find("\r\n");
	if (i == std::string::npos)
		std::cout << "AH" << std::endl;
	if (!this->is_valid(msg.substr(0, i)))
		return ("HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCYPE html>\r\n<html><head><title>404</title></head>\r\n<body><center><h1>404 Not Found</h1><br></center></body></html>\r\n");
	return (this->request);
}

bool	Response::is_valid(std::string demande)
{
	std::string::size_type i;
	std::ifstream	file;
	std::streampos	size;

	i = demande.find("/");
	if (i != std::string::npos)
	{
		std::cout << demande.substr(i) << std::endl;
		file.open("info.html", std::ios::binary);
		if (!file.is_open())
			return false;
		file.seekg(0, std::ios::end);
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		this->body = std::vector<unsigned char> (size);
		file.read((char *)&this->body[0], size);
		this->size_body = size;
		this->request = "HTTP/1.1 200 OK\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n";
		return true;
	}
	return false;
}

const std::vector<unsigned char>& Response::get_body(void) const
{
	return (this->body);
}

	int	Response::get_size(void) const
{
	return this->size_body;
}

Response::Response(const Response& t) : request(t.request)
{
	return;
}

/*std::ostream	&operator<<(std::ostream &os, const Response &d)
{
	os << d.name << std::endl;
	return (os);
} */

Response& Response::operator=(const Response& t)
{
	this->request = t.request;
	return (*this);
}

Response::~Response(void)
{
	std::cout << "destructed" << std::endl;
	return;
}

