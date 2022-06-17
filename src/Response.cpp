#include "Response.hpp"
#include "Request.hpp"
#include "cgi.hpp"
#include "parser_utils.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
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

std::string find_error_page(int status)
{
	switch (status)
	{
		case 400:
			return "errorPages/400.html";
		case 403:
			return "errorPages/403.html";
		case 404:
			return "errorPages/404.html";
		case 405:
			return "errorPages/405.html";
		case 413:
			return "errorPages/413.html";
		case 500:
			return "errorPages/500.html";
		default:
			return "";
	}
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
	if (!req.is_valid()) status = 400;
	else
		this->filename = req.getRoute();
	if (filename.empty()) status = 404;
	else
		status = findRoute(tmp, methods);
	if (status != 200) goto build_response;

	if (ft_stoi(req.getContentLength()) > this->r.getMaxBodySize())
		status = 413;
	temp = this->r.getRoute();
	if (this->r.getUrl().size() == 1) temp += '/';
	temp += this->filename.substr(this->r.getUrl().size());
	this->filename = temp;
	if (isDirectory(this->filename.c_str()))
	{
		if (!r.getDefaultFile().empty() &&
			is_valid(filename + r.getDefaultFile().substr(1)))
			this->redirection(r.getDefaultFile(), req.getRoute());
		else if (r.getBrowse())
		{
			if (methods != GET)
			{
				status = 405;
				goto build_response;
			}
			std::vector<char *> argv;
			argv.push_back((char *) "tree");
			argv.push_back((char *) "-L");
			argv.push_back((char *) "1");
			argv.push_back((char *) "-a");
			argv.push_back((char *) "-C");
			argv.push_back((char *) "-H");
			argv.push_back((char *) ".");
			argv.push_back((char *) this->filename.c_str());
			argv.push_back(NULL);

			int fd[2];
			pid_t child;

			if (pipe(fd) == -1) return;
			child = fork();
			if (child == -1)
			{
				std::cerr << "Fork failed... exiting now.\n";
				status = 500;
				goto build_response;
			}
			if (child == 0)
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1) exit(EXIT_FAILURE);
				if (execvp("tree", &argv[0]) == -1)
				{
					std::cerr << "Error launching tree" << std::endl;
					exit(127);
				}
			}
			close(fd[1]);
			if (fd[0] == -1)
			{
				status = 500;
				goto build_response;
			}
			char *body = new char[bodySize]();
			ssize_t i;
			while ((i = read(fd[0], body, bodySize)) != 0)
			{
				if (i == -1)
				{
					close(fd[0]);
					status = 500;
					goto build_response;
				}
				this->response.appendBody(body);
			}
			close(fd[0]);
			status = 200;
			this->response.setProtocol("HTTP/1.1");
			this->response.setMethod(createStatusLine(status));
			this->response.setDate(Response::Date());
			this->response.setContentType(findType(".html"));
		} else
		{
			status = 404;
			goto build_response;
		}
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
	if (methods == DELETE)
	{
		delete_method(status);
		this->response.setProtocol("HTTP/1.1");
		this->response.setMethod(createStatusLine(status));
		this->response.setDate(Response::Date());
		return;
	}
	if (r.getCGI())
	{
		status = this->callCGI(req, bodySize);
		if (status == 200) return;
		goto build_response;
	}
	if (!is_valid(filename)) status = 404;

build_response:
	if (status != 200)
	{
		if (req.getRoute().find(".jpg") != std::string::npos)
		{
			this->response.setProtocol("HTTP/1.1");
			this->response.setMethod(createStatusLine(302));
			this->response.setDate(Response::Date());
			this->response.setLocation("https://cdn.intra.42.fr" +
									   req.getRoute());
			this->response.setConnection("close");
			return;
		}
		filename = find_error_page(status);
	}
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
	req = route + location;
	if (route.size() == 1) req = location;
	this->response.setLocation(req);
	this->response.setConnection("close");
}

void Response::delete_method(int &status)
{
	status = 200;
	if (remove(this->filename.c_str())) { status = 403; }
}

void Response::put_method(const Request &req, int &status)
{
	std::ofstream file;

	std::ifstream tfile(filename.c_str());
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
			return (200);
		}
		it++;
	}

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
	SLmap[302] = "302 Found";
	SLmap[502] = "502 Bad Gateway";
	SLmap[500] = "500 Internal Server Error";
	SLmap[413] = "413 Request Entity Too Large";
	it = SLmap.find(code);
	if (it == SLmap.end()) return (SLmap[404]);
	return (it->second);
}

void Response::form_body(const std::string &path)
{
	std::ifstream file(path.c_str());
	std::streampos size;

	if (file.fail()) size = 0;
	else
	{
		file.seekg(0, std::ios::end);
		size = file.tellg();
		file.seekg(0, std::ios::beg);
	}
	std::vector<unsigned char> body(size);

	file.read((char *) &body[0], size);
	this->response.setBody(body);
	this->response.setContentLength(ft_itos(size));
}

std::string Response::findType(const std::string &req)
{
	std::map<std::string, std::string> extension;
	std::map<std::string, std::string>::iterator it;
	std::string ret;

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

int Response::callCGI(const Request &req, const int &bodySize)
{
	int fd[2];
	std::string buffer;
	buffer.reserve(bodySize);
	std::map<std::string, std::string> meta_var = Response::buildCGIEnv(req);

	if (pipe(fd) == -1)
	{
		std::cerr << "pipe failed on cgi";
		return 500;
	}
	ssize_t i =
		write(fd[1], &req.getBody()[0], ft_stoi(req.getContentLength()));
	close(fd[1]);

	if (i == 0)
	{
		close(fd[0]);
		return 500;
	}
	if (i == -1)
	{
		close(fd[0]);
		return 500;
	}

	if (!get_gci(buffer, this->filename, fd, meta_var, bodySize))
	{
		close(fd[0]);
		return 500;
	}

	close(fd[0]);

	int status;
	wait(&status);

	if (status) return 500;
	this->response.fillHeader(buffer);
	return (200);
}

std::map<std::string, std::string> Response::buildCGIEnv(const Request &req)
{
	std::map<std::string, std::string> meta_var;

	meta_var["AUTH_TYPE"] = req.getAuthorization();
	meta_var["CONTENT_LENGTH"] = req.getContentLength();
	meta_var["CONTENT_TYPE"] = req.getContentType();
	meta_var["GATEWAY_INTERFACE"] = "CGI/1.1";
	meta_var["PATH_INFO"] = req.getRoute();
	meta_var["PATH_TRANSLATED"] = filename;
	meta_var["QUERY_STRING"] = req.getQueryString();
	meta_var["REMOTE_HOST"] = req.getServerName();
	meta_var["REQUEST_METHOD"] = req.getMethod();
	meta_var["SERVER_NAME"] = req.getServerName();
	meta_var["SERVER_PORT"] = req.getServerPort();
	meta_var["SERVER_PROTOCOL"] = "HTTP/1.1";
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

Response::~Response() {}
