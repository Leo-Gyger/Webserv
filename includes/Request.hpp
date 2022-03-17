//
// Created by Mano Segransan on 3/17/22.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include <iostream>

class Request
{
public:
	Request(std::string &header, std::string &svName, int svPort);
	~Request();

	std::string getMethod() const;
	std::string getRoute() const;
	std::string getProtocol() const;
	std::string getUserAgent() const;
	std::string getAuthorization() const;
	std::string getContentLength() const;
	std::string getContentType() const;
	std::string getQueryString() const;
	std::string getServerName() const;
	std::string getServerPort() const;
	std::string getBody() const;

private:
	std::string method;
	std::string route;
	std::string protocol;
	std::string userAgent;
	std::string authorization;
	std::string contentLength;
	std::string contentType;
	std::string queryString;
	std::string serverName;
	std::string serverPort;
	std::string body;
};


#endif//WEBSERV_REQUEST_HPP
