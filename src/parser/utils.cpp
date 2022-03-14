#include "colours.hpp"
#include "parser_utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>

bool isnotspace(int i) { return !std::isspace(i); }

bool isnotdigit(int i) { return !std::isdigit(i); }

std::string remove_comments(std::string &line)
{
	size_t i;

	i = line.find('#');
	if (i != std::string::npos) line.erase(i);

	return (line);
}

size_t trim_left(std::string &line)
{
	std::string::iterator it =
		std::find_if(line.begin(), line.end(), isnotspace);
	line.erase(line.begin(), it);
	return std::distance(line.begin(), it);
}

int ft_stoi(const std::string &s)
{
	int i;
	if (std::istringstream(s) >> i) return (i);
	return (-1);
}

void parse_error(t_file &f, const std::string &error)
{
	f.file->close();
	std::cerr << BOLD << f.filename << ":" << f.i << ":" << f.j << RED
			  << " error: " << RESET << BOLD << error << RESET << std::endl;
	exit(EXIT_FAILURE);
}

size_t trim_left_number(std::string &line)
{
	std::string::iterator it =
		std::find_if(line.begin(), line.end(), isnotdigit);
	line.erase(line.begin(), it);
	return std::distance(line.begin(), it);
}

int skip_lines(t_file &f)
{
	while (f.file->good() && !f.file->eof())
	{
		++(f.i);
		f.j = 1;
		std::getline(*f.file, f.line);
		remove_comments(f.line);
		f.j += trim_left(f.line);
		if (!f.line.empty()) return (0);
	}
	return (1);
}
