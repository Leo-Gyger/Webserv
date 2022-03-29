//
// Created by Mano Segransan on 3/17/22.
//

#include "Request.hpp"
#include "parser_utils.hpp"
#include <iostream>
#include <string>

Request::Request(std::string &header, std::string &svName, int svPort)
{
	std::istringstream stream(header);
	size_t i;

	this->serverName = svName;
	this->serverPort = ft_itos(svPort);

	std::getline(stream, this->method, ' ');
	//std::cout << this->method << std::endl;
	std::getline(stream, this->route, ' ');
	std::cout << this->route << std::endl;
	i = this->route.find("?");
	if (i != std::string::npos)
	{
		this->queryString = this->route.substr(i, std::string::npos);
		this->route = this->route.substr(0, i);
	}

	std::getline(stream, this->protocol, '\r');

	i = header.find("Authorization: ") +
		std::string("Authorization: ").length();
	this->authorization = header.substr(i, header.find("\r\n", i) - i);

	i = header.find("User-Agent: ") + std::string("User-Agent: ").length();
	this->userAgent = header.substr(i, header.find("\r\n", i) - i);

	i = header.find("Content-Type: ") + std::string("Content-Type: ").length();
	this->contentType = header.substr(i, header.find("\r\n", i) - i);

	i = header.find("Content-Length: ") +
		std::string("Content-Length: ").length();
	if (i != std::string::npos)
	{
		this->contentLength = header.substr(i, header.find("\r\n", i) - i);
		this->body =
			header.substr(header.find("\r\n\r\n", i) + 4, std::string::npos);
	}
	else 
	{
		i = header.find("\r\n\r\n");
		this->body = header.substr(i, std::string::npos);
	}
	return;
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
std::string Request::getBody() const { return this->body; }

int Request::getIntMethod() const {
	if (this->method == "GET")
		return (GET);
	if (this->method == "POST")
		return (POST);
	if (this->method == "DELETE")
		return (DELETE);
	return (-1);
}
