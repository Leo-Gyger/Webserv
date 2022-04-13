#include "Response.hpp"
#include "Request.hpp"
#include "cgi.hpp"
#include "parser_utils.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

bool mySort(const Route &A, const Route &B)
{
	return (A.getRoute().size() > B.getRoute().size());
}

bool filterMethod(const Route &R, int method)
{
	return (R.getMethods() & method);
}

Response::Response(const std::vector<Route> &route, const Request &req,
				   const int &bodySize)
	: response()
{
	bool is_dir;
	int status = 200;
	int methods = req.getIntMethod();
	std::vector<Route> tmp = route;

	for (std::vector<Route>::iterator it = tmp.begin(); it != tmp.end();)
	{
		if (!filterMethod(*it, methods)) it = tmp.erase(it);
		else
			++it;
	}
	if (req.getMethod() == "POST")
	{
		(void) req;
	}
	std::sort(tmp.begin(), tmp.end(), mySort);
	this->filename = req.getRoute();
	if (!findRoute(tmp))
	{
		status = 404;
		filename = "errorPages/404.html";
	}
	if (ft_stoi(req.getContentLength()) > this->r.getMaxBodySize())
	{
		status = 413;
	}
	is_dir = *(this->filename.end() - 1) == '/';
	if (is_dir)
	{
		this->redirection(r.getDefaultFile());
		return;
	}
	if (!is_valid(filename))
	{
		status = 404;
		filename = "errorPages/404.html";
	}
	if (r.getCGI() && status == 200)
	{
		this->callCGI(req, bodySize);
		return;
	}

	form_body(filename);

	this->response.setProtocol("HTTP/1.1");
	this->response.setMethod(createStatusLine(status));
	this->response.setDate(Response::Date());
	this->response.setContentType(findType(filename));
}

void Response::redirection(const std::string &location)
{
	std::string ret;

	this->response.setProtocol("HTTP/1.1");
	this->response.setMethod(createStatusLine(301));
	this->response.setDate(Response::Date());
	this->response.setLocation(location);
	this->response.setConnection("close");
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

bool Response::findRoute(const std::vector<Route> &route)
{
	for (std::vector<Route>::size_type i = 0; i != route.size(); i++)
	{
		std::string::size_type pos;
		pos = this->filename.find(route[i].getUrl());
		if (pos != std::string::npos)
		{
			std::cout << route[i].getUrl() << std::endl;
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
	SLmap[502] = "502 Bad Gateway";
	SLmap[413] = "413 Request Entity Too Large";
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
	std::vector<unsigned char> body(size);

	file.read((char *) &body[0], size);
	this->response.setBody(body);
	this->response.setContentLength(ft_itos(size));
}

std::string Response::findType(std::string req)
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
	i = req.find('.');
	n = req.find(' ', i);
	std::istringstream ss(req.substr(i, n));
	std::getline(ss, req, ' ');
	it = extension.find(req);
	if (it == extension.end()) ret = "application/octet-stream";
	else
		ret = it->second;
	if (req == ".html" || req == ".css" || req == ".ts")
		ret += "; charset=UTF-8";
	return (ret);
}

void Response::callCGI(const Request &req, const int &bodySize)
{
	int fd[2];
	std::string buffer;
	buffer.reserve(bodySize);
	std::map<std::string, std::string> meta_var = Response::buildCGIEnv(req);

	if (pipe(fd) == -1) exit(EXIT_FAILURE);
	write(fd[1], &req.getBody()[0], ft_stoi(req.getContentLength()));
	close(fd[1]);

	std::cout << "ROUTE NAME: "
			  << this->r.getRoute() +
					 req.getRoute().substr(this->r.getUrl().length(),
										   std::string::npos)
			  << std::endl;

	std::cout << this->filename << std::endl;

	if (!get_gci(buffer, this->filename, fd, meta_var, bodySize)) return;

	int status;
	wait(&status);

	if (!status)
	{
		this->response.fill(buffer, req.getServerName(), 8080);
		std::cout << "BUFFER:" << buffer << std::endl;
	}

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


Response::Response(const Response &t) : response(t.response) {}

Request &Response::getResponse() { return this->response; }

std::ostream &operator<<(std::ostream &os, Response &d)
{
	os << d.getResponse().toString() << std::endl;
	return (os);
}

Response &Response::operator=(const Response &t)
{
	this->response = t.response;
	return (*this);
}

Response::~Response() { std::cout << "destructed" << std::endl; }
