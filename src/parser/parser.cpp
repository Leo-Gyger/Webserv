//
// Created by Mano Segransan on 3/7/22.
//

#include <Server.hpp>
#include <algorithm>
#include <colours.hpp>
#include <fstream>
#include <parser.hpp>
#include <sstream>
#include <vector>

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

int stoi(const std::string &s)
{
	int i;
	if (std::istringstream(s) >> i) return (i);
	return (-1);
}

void parse_error(const std::string &filename, std::ifstream &file, size_t &i,
				 const std::string &error)
{
	file.close();
	std::cerr << BOLD << filename << ":" << i << RED << " error: " << RESET
			  << BOLD << error << RESET << std::endl;
	exit(EXIT_FAILURE);
}

std::string trim_left_number(std::string &line)
{
	line.erase(line.begin(),
			   std::find_if(line.begin(), line.end(), isnotdigit));
	return (line);
}

int parse_line(Server &sv, const std::string &filename, std::ifstream &file,
			   std::string &line, size_t &i)
{
	if (line.find('}') == 0) return (1);
	if (line.find("port") == 0)
	{
		line.erase(0, 4);
		trim_left(line);
		sv.setPort(stoi(line));
		trim_left_number(line);
		if (!line.empty()) parse_error(filename, file, i, "garbage at EOL");
	} else
	{
		std::cerr << BOLD
				  << "NOT IMPLEMENTED: UNRECOGNIZED TOKEN IN SERVER DIRECTIVE"
				  << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	return (0);
}

Server parse_server(const std::string &filename, std::ifstream &file,
					std::string &line, size_t &i)
{
	Server sv;

	trim_left(line);
	if (!line.empty())
	{
		if (line.find('}') == 0)
			return (sv);
		else
			parse_error(filename, file, i, "garbage at EOL");
	}
	while (file.good() && !file.eof())
	{
		++i;
		std::getline(file, line);
		remove_comments(line);
		trim_left(line);
		if (!line.empty())
			if (parse_line(sv, filename, file, line, i)) return (sv);
	}
	parse_error(filename, file, i, "missing `}` before EOF");
	return (sv);
}

int skip_lines(std::ifstream &file, std::string &line, size_t &i)
{
	while (file.good() && !file.eof())
	{
		++i;
		std::getline(file, line);
		remove_comments(line);
		trim_left(line);
		if (!line.empty()) return (0);
	}
	return (1);
}

int get_server_directive(const std::string &filename, std::ifstream &file,
						 std::string &line, size_t &i)
{
	if (skip_lines(file, line, i)) { return (1); }
	if (line.find("server") != 0)
		parse_error(filename, file, i, "unrecognized token");
	line.erase(0, 6);
	trim_left(line);
	if (line.empty())
	{
		if (skip_lines(file, line, i))
			parse_error(filename, file, i, "unexpected EOF");
	}
	if (line.find('{') != 0)
		parse_error(filename, file, i, "unrecognized token");
	line.erase(0, 1);
	trim_left(line);
	//	if (!line.empty())
	//		parse_error(filename, file, i, "garbage at EOL");
	return (0);
}

std::vector<Server> parse_config_file(const char *filename)
{
	size_t i = 0;
	std::string line;
	std::ifstream file(filename);
	std::vector<Server> serverList;

	if (!file.is_open()) exit(EXIT_FAILURE);

	if (get_server_directive(filename, file, line, i))
		parse_error(filename, file, i, "could not find server directive");
	serverList.push_back(parse_server(filename, file, line, i));
	while (!get_server_directive(filename, file, line, i))
		serverList.push_back(parse_server(filename, file, line, i));
	file.close();
	return (serverList);
}
