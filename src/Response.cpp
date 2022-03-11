#include "Response.hpp"
#include <fstream>
#include <sstream>
#include <string>
Response::Response(void)
{
	return;
}

std::string	Response::answer(std::string& msg)
{
	std::string::size_type i;

	i = msg.find("\r\n");
	if (i == std::string::npos)
		return ("HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCYPE html>\r\n<html><head><title>404</title></head>\r\n<body><center><h1>404 Not Found</h1><br></center></body></html>\r\n");
	if (!this->is_valid(msg.substr(0, i)))
		return ("HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCYPE html>\r\n<html><head><title>404</title></head>\r\n<body><center><h1>404 Not Found</h1><br></center></body></html>\r\n");
	return (this->request);
}

bool	Response::is_valid(std::string demande)
{
	std::istringstream s(demande);
	std::streampos	size;
	std::string	name;
	std::string localisation = "WWW/";

	std::getline(s,name, ' ');
	std::getline(s,name, ' ');
	if (name == "/")
		name = "/info.html";
	name = name.substr(1);
	name =  localisation + name;
	std::cout << name << std::endl;
	std::ifstream file(name);
	if (!file)
		return false;
	file.seekg(0, std::ios::end);
	size = file.tellg();
	file.seekg(0, std::ios::beg);
	this->body = std::vector<unsigned char> (size);
	file.read((char *)&this->body[0], size);
	this->size_body = size;
	this->request = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n";
	return true;
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

