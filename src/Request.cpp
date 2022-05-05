//
// Created by Mano Segransan on 3/17/22.
//

#include "Request.hpp"
#include "parser_utils.hpp"
#include <algorithm>
#include <iostream>
#include <string>

Request::Request() {}

Request::Request(const std::string &header, const std::string &svName,
				 const int &svPort)
{
	this->fill(header, svName, svPort);
}

void Request::fill(const std::string &header, const std::string &svName,
				   const int &svPort)
{
	this->content = header;
	std::istringstream stream(header);
	size_t i;

	this->serverName = svName;
	this->serverPort = ft_itos(svPort);

	std::getline(stream, this->method, ' ');
	std::getline(stream, this->route, ' ');
	i = this->route.find('?');
	if (i != std::string::npos)
	{
		this->queryString = this->route.substr(i, std::string::npos);
		this->route = this->route.substr(0, i);
	}
	if (*(this->route.end() - 1) == '/' && this->route.size() != 1)
		this->route = this->route.substr(0, this->route.size() - 1);

	std::getline(stream, this->protocol, '\r');
	i = header.find("Authorization: ");
	if (i != std::string::npos)
	{
		i += std::string("Authorization: ").length();
		this->authorization = header.substr(i, header.find("\r\n", i) - i);
	}

	i = header.find("User-Agent: ");
	if (i != std::string::npos)
	{
		i += std::string("User-Agent: ").length();
		this->userAgent = header.substr(i, header.find("\r\n", i) - i);
	}

	i = header.find("Content-Type: ");
	if (i != std::string::npos)
	{
		i += std::string("Content-Type: ").length();
		this->contentType = header.substr(i, header.find("\r\n", i) - i);
	}

	i = header.find("Content-Length: ");
	if (i != std::string::npos)
	{
		i += std::string("Content-Length: ").length();
		this->contentLength = header.substr(i, header.find("\r\n", i) - i);
		if (ft_stoi(this->contentLength) == -1) throw std::exception();
		std::string val("\r\n\r\n");
		std::string::iterator it =
			std::find_first_of(this->content.begin(), this->content.end(),
							   val.begin(), val.end()) +
			(long) val.length();
		this->body.insert(this->body.begin(), it, this->content.end());
		if (this->body.size() < (unsigned int) ft_stoi(this->contentLength))
			throw std::exception();
	}
}

Request::~Request() {}

std::string Request::getMethod() const { return this->method; }
std::string Request::getRoute() const { return this->route; }
std::string Request::getProtocol() const { return this->protocol; }
std::string Request::getUserAgent() const { return this->userAgent; }
std::string Request::getAuthorization() const { return this->authorization; }
std::string Request::getContentLength() const { return this->contentLength; }
std::string Request::getContentType() const { return this->contentType; }
std::string Request::getQueryString() const { return this->queryString; }
std::string Request::getServerName() const { return this->serverName; }
std::string Request::getServerPort() const { return this->serverPort; }
std::vector<unsigned char> Request::getBody() const { return this->body; }

int Request::getIntMethod() const
{
	if (this->method == "GET") return (GET);
	if (this->method == "POST") return (POST);
	if (this->method == "DELETE") return (DELETE);
	if (this->method == "PUT") return (PUT);
	if (this->method == "HEAD") return (HEAD);
	return (-1);
}

std::string Request::toString() const
{
	std::string ret = this->protocol + " " + this->method +
					  "\r\n"
					  "Date: " +
					  this->date + "\r\n";

	if (this->method.find("405") == 0) ret += "Allow: " + this->allow + "\r\n";
	if (!this->location.empty()) ret += "Location: " + this->location + "\r\n";
	if (!this->contentType.empty())
		ret += "Content-Type: " + this->contentType + "\r\n";
	if (!this->contentLength.empty())
		ret += "Content-Length: " + this->contentLength + "\r\n";
	if (!this->connection.empty())
		ret += "Connection: " + this->connection + "\r\n";
	ret += "\r\n";
	return (ret);
}

void Request::setMethod(const std::string &m) { this->method = m; }
void Request::setRoute(const std::string &r) { this->route = r; }
void Request::setProtocol(const std::string &p) { this->protocol = p; }
void Request::setUserAgent(const std::string &ua) { this->userAgent = ua; }
void Request::setAuthorization(const std::string &a)
{
	this->authorization = a;
}
void Request::setContentLength(const std::string &cl)
{
	this->contentLength = cl;
}
void Request::setContentType(const std::string &ct) { this->contentType = ct; }
void Request::setQueryString(const std::string &qs) { this->queryString = qs; }
void Request::setServerName(const std::string &sn) { this->serverName = sn; }
void Request::setServerPort(const std::string &sp) { this->serverPort = sp; }
void Request::setBody(const std::vector<unsigned char> &b)
{
	this->body.insert(this->body.begin(), b.begin(), b.end());
}
void Request::setDate(const std::string &d) { this->date = d; }
void Request::setLocation(const std::string &l) { this->location = l; }
void Request::setConnection(const std::string &c) { this->connection = c; }
void Request::setAllow(const std::string &a) { this->allow = a; }
