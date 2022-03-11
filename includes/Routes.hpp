//
// Created by Mano Segransan on 3/11/22.
//

#ifndef WEBSERV_ROUTES_HPP
#define WEBSERV_ROUTES_HPP

#include <iostream>

#define GET 1 // 0b1
#define POST 2 // 0b10
#define DELETE 4 // 0b100

class Routes
{
public:
	Routes();
	~Routes();

	void setMethods(int p);
	int getMethods() const;

	void addMethods(int p);
	void delMethods(int p);

	void setUrl(const std::string& u);
	std::string getUrl() const;

	void setPath(const std::string& p);
	std::string getPath() const;
private:
	std::string url;
	std::string path;
	int methods;
};


#endif//WEBSERV_ROUTES_HPP
