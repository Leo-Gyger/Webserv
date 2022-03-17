#include "Response.hpp"
#include <fstream>
#include <sstream>
#include <string>

struct	sortLString{
	bool operator() (Routes A, Routes B) const
	{
		return (A.getRoute().size() > B.getRoute().size());
	}
} mySort;


Response::Response(const std::vector<Routes>& routes, const std::string& header) : size_body()
{
	bool	is_dir = false;
	int	status = 200;
	bool	debug = false;
	std::vector<Routes> tmp = routes;

	std::sort(tmp.begin(),tmp.end(),mySort);
	this->body.clear();
	this->filename = createFname(header, is_dir);
	if (!findRoute(tmp, filename))
	{
		status = 404;
		filename = "errorPages/404.html";
	}
	if (is_dir)
	{
		this->request = redirection(r.getDefaultFile());
		return;
	}
	if (!is_valid(filename))
	{
		status = 404;
		filename = "errorPages/404.html";
	}
	if (debug != false)
	{
		this->request = redirection("https://datatracker.ietf.org/doc/html/rfc7231#section-7.1.2");
		return;
	}
	form_body(filename);
	this->request = "HTTP/1.1 " + createStatusLine(status) +
					"\r\n"
					"Date: " +
					this->Date() +
					"\r\n"
					"Content-Length: " +
					this->get_bSize() +
					"\r\n"
					"Content-Type: " +
					this->findType(filename) +
					"\r\n"
					"charset=UTF-8\r\n\r\n";
}

std::string Response::redirection(const std::string&	location)
{
	std::string ret;

	ret = "HTTP/1.1 " + createStatusLine(301) +
		  "\r\n"
		  "Date: " +
		  this->Date() +
		  "\r\n"
		  "Location: " +
		  location +
		  "\r\n"
		  "Connection: close\r\n"
		  "\r\n";
	return (ret);
} 

std::string	Response::createFname(const std::string &header, bool& is_dir)
{
	std::istringstream	stream(header);
	std::string name;
	std::string::iterator	it;

	std::getline(stream, name, ' ');
	std::getline(stream, name, ' ');
	it = name.end() - 1;
	if (*it == '/')
		is_dir = true;
	return (name);
}

bool	Response::findRoute(const std::vector<Routes>& routes, const std::string& file_name)
{
	for (std::vector<Routes>::size_type i = 0; i != routes.size(); i++)
		std::cout << routes[i].getRoute() << std::endl;
		for	(std::vector<Routes>::size_type	i = 0; i != routes.size(); i++)
		{
			std::string::size_type	pos;
			pos = file_name.find(routes[i].getUrl());
			if (pos != std::string::npos)
			{
				this->r = routes[i];
				return true;
			}
		}
	return false;
}

bool Response::is_valid(std::string &demande)
{
	bool ret_val = true;

	demande = this->r.getRoute() + demande.substr(1);
	std::ifstream file(demande.c_str());
	if (!file)
		ret_val = false;
	file.close();
	return ret_val;
}

std::string Response::createStatusLine(int code)
{
	std::map<int, std::string> SLmap;
	std::map<int, std::string>::iterator it;

	SLmap[200] = "200 OK";
	SLmap[404] = "404 Not Found";
	SLmap[201] = "201 Created";
	SLmap[301] = "301 Moved Permanently";
	it = SLmap.find(code);
	if (it == SLmap.end()) return (SLmap[404]);
	return (it->second);
}

void Response::form_body(const std::string &path)
{
	std::ifstream file(path.c_str());
	std::streampos size;

	file.seekg(0, std::ios::end);
	size = file.tellg();
	file.seekg(0, std::ios::beg);
	this->body = std::vector<unsigned char>(size);
	file.read((char *) &this->body[0], size);
	this->size_body = (int) size;
}

std::string Response::get_bSize() const
{
	std::stringstream s;

	s << this->size_body;
	return (std::string(s.str()));
}

std::string Response::findType(std::string demande)
{
	std::map<std::string, std::string> extension;
	std::map<std::string, std::string>::iterator it;
	std::string ret;

	extension[".jpg"] = "image/jpeg";
	extension[".ico"] = "image/x-icon";
	extension[".html"] = "text/html";
	extension[".css"] = "text/css";
	extension[".avi"] = "video/x-msvideo";
	extension[".gif"] = "image/gif";
	extension[".ts"] = "application/typescript";
	std::string::size_type i, n;
	i = demande.find('.');
	n = demande.find(' ', i);
	std::istringstream ss(demande.substr(i, n));
	std::getline(ss, demande, ' ');
	it = extension.find(demande);
	if (it == extension.end()) ret = "application/octet-stream";
	else
		ret = it->second;
	return (ret);
}


std::string Response::Date()
{
	char buf[1000] = {0};
	std::stringstream ss;
	std::time_t t = time(nullptr);

	std::tm tm = *gmtime(&t);
	strftime(buf, sizeof(buf), "%a %d %b %Y %H:%M:%S %Z", &tm);
	ss << buf;
	std::string str(ss.str());
	return (str);
}

const std::vector<unsigned char> &Response::get_body() const
{
	return (this->body);
}

int Response::get_size() const { return this->size_body; }

Response::Response(const Response &t)
	: request(t.request), size_body()
{}

const std::string &Response::getRequest() const { return this->request; }

std::ostream &operator<<(std::ostream &os, const Response &d)
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
