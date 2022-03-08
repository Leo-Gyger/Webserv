//
// Created by Mano Segransan on 3/7/22.
//

#include <Server.hpp>
#include <algorithm>
#include <colours.hpp>
#include <fstream>
#include <parser.hpp>
#include <vector>

//void parse_line(Server &s, std::string line)
//{
//
//}

Server parse_server(std::ifstream &file,
					std::string &line, size_t &i)
{
	Server s;

	(void )file;
	(void )line;
	(void )i;
	//
//	while (file.good() && !file.eof())
//	{
//		std::getline(file, line);
//	}
	return (s);
}

struct not_space : std::unary_function<int, bool> {
	bool operator()(int i) const { return !std::isspace(i); }
};

std::string trim_line(std::string &line)
{
	size_t i;

	i = line.find('#');
	if (i != std::string::npos) line.erase(i);
	line.erase(line.begin(),
			   std::find_if(line.begin(), line.end(), not_space()));
	return (line);
}

int skip_lines(std::ifstream &file, std::string &line, size_t &i)
{
	while (file.good() && !file.eof())
	{
		++i;
		std::getline(file, line);
		trim_line(line);
		if (!line.empty()) return (0);
	}
	return (1);
}

int get_server_directive(const std::string &filename, std::ifstream &file,
						 std::string &line, size_t &i)
{
	if (skip_lines(file, line, i)) { return (1); }
	if (line.find("server") != 0)
	{
		std::cerr << BOLD << filename << ":" << i << RED << " error:" << RESET
				  << BOLD << " unrecognized token" << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	line.erase(0, 6);
	trim_line(line);
	if (line.empty())
	{
		if (skip_lines(file, line, i))
		{
			std::cerr << BOLD << filename << ":" << i << RED
					  << " error:" << RESET << BOLD << " unexpected EOF"
					  << RESET << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	if (line.find('{') != 0)
	{
		std::cerr << BOLD << filename << ":" << i << RED << " error:" << RESET
				  << BOLD << " unrecognized token" << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	line.erase(0, 1);
	return (0);
}

void parse_config_file(const std::string &filename)
{
	size_t i = 0;
	std::string line;
	std::ifstream file(filename);
	std::vector<Server> serverList;

	if (!file.is_open()) exit(EXIT_FAILURE);

	if (get_server_directive(filename, file, line, i))
	{
		std::cerr << BOLD << filename << ":" << i << RED << " error:" << RESET
				  << BOLD << " could not find server directive" << RESET
				  << std::endl;
		exit(EXIT_FAILURE);
	}
	serverList.push_back(parse_server(file, line, i));
	while (!get_server_directive(filename, file, line, i))
		serverList.push_back(parse_server(file, line, i));

	std::cout << RED << line << RESET << std::endl;
	file.close();
}
