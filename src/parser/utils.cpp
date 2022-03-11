#include "colours.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "parser_utils.hpp"

bool isnotspace(int i) { return !std::isspace(i); }


bool isnotdigit(int i) { return !std::isdigit(i); }

std::string remove_comments(std::string &line)
{
	size_t i;

	i = line.find('#');
	if (i != std::string::npos) line.erase(i);

	return (line);
}

std::string trim_left(std::string &line)
{
	line.erase(line.begin(),
			   std::find_if(line.begin(), line.end(), isnotspace));
	return (line);
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
	std::cerr << BOLD << f.filename << ":" << f.i << RED << " error: " << RESET
			  << BOLD << error << RESET << std::endl;
	exit(EXIT_FAILURE);
}

std::string trim_left_number(std::string &line)
{
	line.erase(line.begin(),
			   std::find_if(line.begin(), line.end(), isnotdigit));
	return (line);
}

int skip_lines(t_file &f)
{
	while (f.file->good() && !f.file->eof())
	{
		++(f.i);
		std::getline(*f.file, f.line);
		remove_comments(f.line);
		trim_left(f.line);
		if (!f.line.empty()) return (0);
	}
	return (1);
}
