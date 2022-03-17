//
// Created by Mano Segransan on 3/15/22.
//

#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include <map>

int get_gci(std::string &buffer, const std::string &filepath, int in[2],
			const std::map<std::string, std::string> &meta_var);

#endif//WEBSERV_CGI_HPP
