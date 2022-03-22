#include "Response.hpp"
#include "Request.hpp"
#include "cgi.hpp"
#include "parser_utils.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

	bool mySort (Route A, Route B)
	{
		return (A.getRoute().size() > B.getRoute().size());
	}

Response::Response(const std::vector<Route> &route, const Request &req)
	: body(), size_body()
{
	bool is_dir;
	int status = 200;

  std::vector<Route> tmp = route;
  std::sort(tmp.begin(),tmp.end(),mySort);
	this->filename = req.getRoute();
	if (!findRoute(tmp, filename, req))
	{
		status = 404;
		filename = "errorPages/404.html";
	}
  is_dir = *(this->filename.end() - 1) == '/';
	if (is_dir)
	{
		std::cout << "entered" << std::endl;
		this->request = redirection(r.getDefaultFile());
		return;
	}
	if (r.getCGI())
	{
		this->request = Response::callCGI(req);
		return;
	}
	if (!is_valid(filename))
	{
		status = 404;
		filename = "errorPages/404.html";
	}
	form_body(filename);
	this->request = "HTTP/1.1 " + createStatusLine(status) +
					"\r\n"
					"Date: " +
					Date() +
					"\r\n"
					"Content-Length: " +
					ft_itos(this->size_body) +
					"\r\n"
					"Content-Type: " +
					findType(filename) +
					"\r\n"
					"charset=UTF-8\r\n\r\n";
}

std::string Response::redirection(const std::string &location)
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

std::string Response::createFname(const std::string &header, bool &is_dir)
{
	std::istringstream stream(header);
	std::string name;
	std::string::iterator it;

	std::getline(stream, name, ' ');
	std::getline(stream, name, ' ');
	it = name.end() - 1;
	if (*it == '/') is_dir = true;
	return (name);
}

bool Response::findRoute(const std::vector<Route> &route,
						 const std::string &file_name, const Request&	req)
{
		for	(std::vector<Route>::size_type	i = 0; i != route.size(); i++)
		{
			std::string::size_type	pos;
			pos = file_name.find(route[i].getUrl());
			if (pos != std::string::npos && req.getMethod() == "GET")
			{
				//std::cout << route[i].getUrl() << std::endl;
				this->r = route[i];
				return true;
			}
		}
	return false;
}

bool Response::is_valid(std::string &demande)
{
	bool ret_val = true;

	demande = this->r.getRoute() + demande.substr(this->r.getUrl().size());
	//std::cout << demande << std::endl;
	std::ifstream file(demande.c_str());
	if (!file) ret_val = false;
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

std::string Response::callCGI(const Request &req)
{
	int fd[2];
	std::string buffer;

	std::map<std::string, std::string> meta_var = Response::buildCGIEnv(req);

	if (pipe(fd) == -1) exit(EXIT_FAILURE);
	write(fd[1], req.getBody().c_str(), req.getBody().length());
	close(fd[1]);
	get_gci(
		buffer,
		this->r.getRoute() +
			req.getRoute().substr(this->r.getUrl().length(), std::string::npos),
		fd, meta_var);
	std::cout << "BUFFER:" << buffer << std::endl;
	return (buffer);
}

std::map<std::string, std::string> Response::buildCGIEnv(const Request &req)
{
	std::map<std::string, std::string> meta_var;

	meta_var["AUTH_TYPE"] = req.getAuthorization();
	meta_var["CONTENT_LENGTH"] = req.getContentLength();
	meta_var["CONTENT_TYPE"] = req.getContentType();
	meta_var["GATEWAY_INTERFACE"] = "CGI/1.1";
	meta_var["PATH_INFO"] = "";      // todo
	meta_var["PATH_TRANSLATED"] = "";// todo
	meta_var["QUERY_STRING"] = req.getQueryString();
	meta_var["REMOTE_ADDR"] = ""; // todo
	meta_var["REMOTE_HOST"] = ""; // todo
	meta_var["REMOTE_IDENT"] = "";// todo
	meta_var["REMOTE_USER"] = ""; // todo
	meta_var["REQUEST_METHOD"] = req.getMethod();
	meta_var["SCRIPT_NAME"] = "";// todo
	meta_var["SERVER_NAME"] = req.getServerName();
	meta_var["SERVER_PORT"] = req.getServerPort();
	meta_var["SERVER_PROTOCOL"] = "HTTP/1.1";
	meta_var["SERVER_SOFTWARE"] = "";// todo
	meta_var["QUERY_STRING"] = req.getQueryString();

	return (meta_var);
}

std::string Response::Date()
{
	char buf[1000] = {0};
	std::stringstream ss;
	std::time_t t = time(NULL);

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

Response::Response(const Response &t) : request(t.request), size_body() {}

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
