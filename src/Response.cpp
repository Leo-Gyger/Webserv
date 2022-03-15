#include "Response.hpp"
#include <fstream>
#include <sstream>
#include <string>

Response::Response() : size_body() {}

std::string Response::answer(std::string &msg)
{
	std::string::size_type i;

	i = msg.find("\r\n");
	if (i == std::string::npos)
		return ("HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: "
				"text/html; charset=UTF-8\r\n\r\n<!DOCTYPE "
				"html>\r\n<html><head><title>404</title></"
				"head>\r\n<body><center><h1>404 Not "
				"Found</h1><br></center></body></html>\r\n");
	if (!this->is_valid(msg.substr(0, i)))
		return ("HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Type: "
				"text/html; charset=UTF-8\r\n\r\n<!DOCTYPE "
				"html>\r\n<html><head><title>404</title></"
				"head>\r\n<body><center><h1>404 Not "
				"Found</h1><br></center></body></html>\r\n");
	return (this->request);
}

bool Response::is_valid(const std::string &demande)
{
	std::istringstream s(demande);
	std::streampos size;
	int	status = 200;
	bool ret_val = true;
	std::string name;
	std::string localisation = "www/";

	std::getline(s, name, ' ');
	std::getline(s, name, ' ');
	if (name == "/") name = "/info.html";
	name = name.substr(1);
	name = localisation + name;
	std::ifstream file(name.c_str());
	file.close();
	if (!file)
	{
		name = "./errorPages/404.html";
		status = 404;
		ret_val = false;
	}
	form_body(name);
	this->findType(name);
	this->request =
		"HTTP/1.1 " + createStatusLine(status) + "\r\n"
		"Date: " + this->Date() + "\r\n"
		"Content-Length: " + this->get_bSize() + "\r\n"
		"Content-Type: " + this->findType(name) + "\r\n"
		"charset=UTF-8\r\n\r\n";
	std::cout << this->request << std::endl;
	return ret_val;
}

std::string	Response::createStatusLine(int	code)
{
	std::map<int, std::string> SLmap;
	std::map<int, std::string>::iterator it;

	SLmap[200] = "200 OK";
	SLmap[404] = "404 Not Found";
	SLmap[201] = "201 Created";
	it = SLmap.find(code);
	if (it == SLmap.end())
		return (SLmap[404]);
	return (it->second);
}

void	Response::form_body(const std::string& path)
{
	std::ifstream	file(path.c_str());
	std::streampos size;

	file.seekg(0, std::ios::end);
	size = file.tellg();
	file.seekg(0, std::ios::beg);
	this->body = std::vector<unsigned char>(size);
	file.read((char *) &this->body[0], size);
	this->size_body = (int)size;
}

std::string	Response::get_bSize(void)
{
	std::stringstream s;

	s << this->size_body;
	return (std::string(s.str()));
}

std::string	Response::findType(std::string demande)
{
	std::map<std::string, std::string> extension;
	std::map<std::string, std::string>::iterator it;
	std::string	ret;

	extension[".jpg"] = "image/jpeg";
	extension[".ico"] = "image/x-icon";
	extension[".html"] = "text/html";
	extension[".css"] = "text/css";
	extension[".avi"] = "video/x-msvideo";
	extension[".gif"] = "image/gif";
	extension[".ts"] = "application/typescript";
	std::string::size_type i,n;
	i = demande.find(".");
	n = demande.find(" ", i);
	std::istringstream ss (demande.substr(i, n));
	std::getline(ss, demande, ' ');
	it = extension.find(demande);
	if (it == extension.end())
		ret = "application/octet-stream";
	else
		ret = it->second;
	return (ret);
}


std::string	Response::Date(void)
{
	std::tm	tm;
	char buf[1000] = {0};
	std::stringstream ss;
	std::time_t	t = time(0);

	tm = *gmtime(&t);
	strftime(buf, sizeof(buf), "%a %d %b %Y %H:%M:%S %Z", &tm);
	ss << buf;
	std::string	str(ss.str());
	return (str);
}

const std::vector<unsigned char> &Response::get_body() const
{
	return (this->body);
}

int Response::get_size() const { return this->size_body; }

Response::Response(const Response &t) : request(t.request), size_body() {}

const std::string&    Response::getRequest(void) const
{
    return this->request;
}

std::ostream	&operator<<(std::ostream &os, const Response &d)
{
	os << d.getRequest() << std::endl;
	return (os);
}

Response &Response::operator=(const Response &t)
{
	this->request = t.request;
	return (*this);
}

Response::~Response() { std::cout << "destructed" << std::endl; }
