//
// Created by Mano Segransan on 3/7/22.
//

#include <parser.hpp>
#include <Server.hpp>
#include <fstream>

std::string	get_config(const std::string& filename)
{
	std::string buffer;
	std::ifstream file (filename);

	if (!file.is_open())
		exit(EXIT_FAILURE);
	file >> buffer;
	file.close();
	return (buffer);
}

void parse_config_file(const std::string& filename)
{
	std::string buffer = get_config(filename);


}
