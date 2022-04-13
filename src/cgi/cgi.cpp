//
// Created by Mano Segransan on 3/15/22.
//

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

// const std::string &name, const std::string &value
void add_env(const std::pair<std::string, std::string>(&meta_var))
{
	setenv(meta_var.first.c_str(), meta_var.second.c_str(), 1);
}

std::string get_gci_executable(const std::string &filepath)
{
	if (!filepath.compare(filepath.length() - 3, 3, ".py")) return ("python3");
	if (!filepath.compare(filepath.length() - 3, 3, ".js")) return ("node");

	std::cerr << "Unrecognized file extension";
	exit(EXIT_FAILURE);
}

int execute_cgi(const std::string &filepath, int in[2],
				const std::map<std::string, std::string> &meta_var)
{
	int fd[2];
	pid_t child;
	std::string exec;

	exec = get_gci_executable(filepath);
	if (pipe(fd) == -1) exit(EXIT_FAILURE);
	child = fork();
	if (child == -1) exit(EXIT_FAILURE);
	if (child == 0)
	{
		std::for_each(meta_var.begin(), meta_var.end(), add_env);
		close(fd[0]);
		close(in[1]);
		if (dup2(fd[1], STDOUT_FILENO) == -1) exit(EXIT_FAILURE);
		if (dup2(in[0], STDIN_FILENO) == -1) exit(EXIT_FAILURE);
		if (execlp(exec.c_str(), exec.c_str(), filepath.c_str(),
				   (char *) NULL) == -1)
		{
			std::cout << "err" << std::endl;
			exit(127);
		}
	}
	close(fd[1]);
	close(in[0]);
	return (fd[0]);
}

int get_gci(std::string &buffer, const std::string &filepath, int in[2],
			const std::map<std::string, std::string> &meta_var)
{
	int fd;
	const int bodySize = 3000;
	char body[bodySize];

	fd = execute_cgi(filepath, in, meta_var);
	while (read(fd, &body, bodySize)) {}
	std::cout << buffer.size() << std::endl;
	return (1);
}
