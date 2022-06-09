//
// Created by Mano Segransan on 3/17/22.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include "Route.hpp"
#include <iostream>
#include <string>

class Request
{
public:
	Request();

	Request(const std::string &, const int &);
	~Request();

	void fill(const std::string &, const int &);
	std::string getMethod() const;
	int getIntMethod() const;
	std::string getRoute() const;
	std::string getProtocol() const;
	std::string getUserAgent() const;
	std::string getAuthorization() const;
	std::string getContentLength() const;
	std::string getContentType() const;
	std::string getQueryString() const;
	std::string getServerName() const;
	std::string getServerPort() const;
	std::vector<unsigned char> getBody() const;
	std::string getDate() const;
	bool isFull() const;
	std::string toString();

	void appendBody(const std::string &bd);
	void fillHeader(const std::string &header);

	void setMethod(const std::string &);
	void setRoute(const std::string &);
	void setAllow(const std::string &);
	void setProtocol(const std::string &);
	void setUserAgent(const std::string &);
	void setAuthorization(const std::string &);
	void setContentLength(const std::string &);
	void setContentType(const std::string &);
	void setQueryString(const std::string &);
	void setServerName(const std::string &);
	void setServerPort(const std::string &);
	void setBody(const std::vector<unsigned char> &);

	void setDate(const std::string &);
	void setLocation(const std::string &);
	void setConnection(const std::string &);
	bool is_valid(void) const;
private:
	std::string method;
	std::string allow;
	std::string route;
	std::string protocol;
	std::string userAgent;
	std::string authorization;
	std::string contentLength;
	std::string contentType;
	std::string queryString;
	std::string serverName;
	std::string serverPort;
	std::vector<unsigned char> body;

	std::string date;
	std::string location;
	std::string connection;

	std::string content;
	std::string hd;
};


#endif//WEBSERV_REQUEST_HPP
