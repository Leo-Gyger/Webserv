#include "Response.hpp"
#include "Request.hpp"
#include "cgi.hpp"
#include "parser_utils.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

bool mySort(const Route &A, const Route &B)
{
	return (A.getRoute().size() > B.getRoute().size());
}

int isDirectory(const char *path)
{
	struct stat statbuf = {};
	if (stat(path, &statbuf) != 0) return 0;
	return S_ISDIR(statbuf.st_mode);
}


Response::Response(const std::vector<Route> &route, const Request &req,
				   const int &bodySize)
	: response()
{
	int status;
	int methods = req.getIntMethod();
	std::vector<Route> tmp = route;
	std::string temp;

	std::sort(tmp.begin(), tmp.end(), mySort);
	this->filename = req.getRoute();
	if (filename.empty()) status = 404;
	else
		status = findRoute(tmp, methods);
	if (status == 404) filename = "errorPages/404.html";
	if (status == 405) filename = "errorPages/405.html";
	if (status != 200) goto build_response;

	if (ft_stoi(req.getContentLength()) > this->r.getMaxBodySize())
	{
		status = 413;
	}
	temp = this->r.getRoute();
	if (this->r.getUrl().size() == 1) temp += '/';
	temp += this->filename.substr(this->r.getUrl().size());
	this->filename = temp;
	std::cout << "FILENAME: " << this->filename << std::endl;
	if (isDirectory(this->filename.c_str()))
	{
		this->redirection(r.getDefaultFile(), req.getRoute());
		return;
	}
	if (methods == PUT)
	{
		put_method(req, status);
		this->response.setProtocol("HTTP/1.1");
		status = 201;
		this->response.setMethod(createStatusLine(status));
		this->response.setDate(Response::Date());
		return;
	}
	if (r.getCGI())
	{
		this->callCGI(req, bodySize);
		return;
	}
	if (!is_valid(filename))
	{
		status = 404;
		filename = "errorPages/404.html";
	}
	if (status == 413)
	{
		filename = "errorPages/413.html";
	}
build_response:
	form_body(filename);

	this->response.setProtocol("HTTP/1.1");
	this->response.setMethod(createStatusLine(status));
	this->response.setDate(Response::Date());
	this->response.setContentType(findType(filename));
}

void Response::redirection(const std::string &location,
						   const std::string &route)
{
	std::string ret;
	std::string req;

	this->response.setProtocol("HTTP/1.1");
	this->response.setMethod(createStatusLine(301));
	this->response.setDate(Response::Date());
	std::cout << "GETURL, redirection: " << this->r.getUrl() << std::endl;
	req = route + location;
	if (route.size() == 1) req = location;
	this->response.setLocation(req);
	this->response.setConnection("close");
}
void Response::put_method(const Request &req, int &status)
{
	std::ofstream file;

	std::ifstream tfile(filename.c_str());
	std::cout << "PUT BODY: " << this->filename << std::endl;
	if (!tfile)
	{
		status = 201;
		file.open(this->filename.c_str(),
				  std::fstream::in | std::fstream::out | std::fstream::trunc);
		file << req.getBody().data();
	} else
	{
		tfile.close();
		status = 200;
		file.close();
		remove(this->filename.c_str());
		file.open(this->filename.c_str(),
				  std::fstream::in | std::fstream::out | std::fstream::trunc);
		file << req.getBody().data();
	}
	file.close();
}

std::string Response::createFname(const std::string &header, bool &is_dir)
{
	std::istringstream stream(header);
	std::string name;
	std::string::iterator it;

	std::getline(stream, name, ' ');
	std::getline(stream, name, ' ');
	if (name.find('.') == std::string::npos) is_dir = true;
	return (name);
}

int Response::findRoute(std::vector<Route> &route, int method)
{
	for (std::vector<Route>::iterator it = route.begin(); it != route.end();)
	{

		if (this->filename.find(it->getUrl()) == 0 &&
			this->filename.length() >= it->getUrl().length())
			it++;
		else
			it = route.erase(it);
	}

	if (route.empty()) return (404);

	int allowedMethods = 0;
	for (std::vector<Route>::iterator it = route.begin(); it != route.end();)
	{
		allowedMethods |= it->getMethods();
		if (it->getMethods() & method)
		{
			this->r = Route(*it);
			std::cout << "GETURL, findRoute: " << it->getUrl() << std::endl;
			return (200);
		}
		it++;
	}

	// todo: we probably need to move this to its own function
	std::string allowed;
	if (allowedMethods & GET) allowed += "GET";
	if (allowedMethods & POST)
	{
		if (!allowed.empty()) allowed += " ,";
		allowed += "POST";
	}
	if (allowedMethods & PUT)
	{
		if (!allowed.empty()) allowed += " ,";
		allowed += "PUT";
	}
	if (allowedMethods & DELETE)
	{
		if (!allowed.empty()) allowed += " ,";
		allowed += "DELETE";
	}
	if (allowedMethods & HEAD)
	{
		if (!allowed.empty()) allowed += " ,";
		allowed += "HEAD";
	}

	this->response.setAllow(allowed);
	return (405);
}

bool Response::is_valid(const std::string &req)
{
	bool ret_val = true;


	std::ifstream file(req.c_str());
	std::cout << "is_valid, request" << req << std::endl;
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
	SLmap[405] = "405 Method Not Allowed";
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
	// todo: segfault if path is empty
	if (path.empty()) return;
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

	// todo: segfault if path is empty
	if (req.empty()) return (std::string());

	extension[".jpg"] = "image/jpeg";
	extension[".ico"] = "image/x-icon";
	extension[".html"] = "text/html";
	extension[".css"] = "text/css";
	extension[".avi"] = "video/x-msvideo";
	extension[".gif"] = "image/gif";
	extension[".ts"] = "application/typescript";
	std::string::size_type i;
	i = req.find('.');
	if (i != std::string::npos) it = extension.find(req.substr(i));
	else
		it = extension.end();
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


	if (!get_gci(buffer, this->filename, fd, meta_var, bodySize)) return;

	int status;
	wait(&status);

	if (!status)
	{
		this->response.fill(buffer, req.getServerName(), 200);
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
