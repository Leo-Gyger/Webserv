//
// Created by Mano Segransan on 3/11/22.
//

#ifndef WEBSERV_PARSER_UTILS_HPP
#define WEBSERV_PARSER_UTILS_HPP

#include <iostream>
#include <sstream>

typedef struct s_file {
	std::string filename;
	std::ifstream *file;
	std::string line;
	size_t i;
	size_t j;
} t_file;

int skip_lines(t_file &f);

size_t trim_left(std::string &line);

int ft_stoi(const std::string &s);

void parse_error(t_file &f, const std::string &error);

size_t trim_left_number(std::string &line);

std::string remove_comments(std::string &line);

std::string ft_itos(size_t i);

#endif//WEBSERV_PARSER_UTILS_HPP
