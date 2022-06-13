//
// Created by Mano Segransan on 3/15/22.
//

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <unistd.h>
#include <vector>

// const std::string &name, const std::string &value
void add_env(const std::pair<std::string, std::string>(&meta_var))
{
	setenv(meta_var.first.c_str(), meta_var.second.c_str(), 1);
}

std::string get_gci_executable(const std::string &filepath)
{
	if (!filepath.compare(filepath.length() - 3, 3, ".py")) return ("python3");
	if (!filepath.compare(filepath.length() - 3, 3, ".js")) return ("node");
	return ("sh");
}

int execute_cgi(const std::string &filepath, int in[2],
				const std::map<std::string, std::string> &meta_var)
{
	int fd[2];
	pid_t child;
	std::string exec;

	exec = get_gci_executable(filepath);
	if (pipe(fd) == -1) return (-1);
	child = fork();
	if (child == -1) exit(EXIT_FAILURE);
	if (child == 0)
	{
		std::for_each(meta_var.begin(), meta_var.end(), add_env);
		close(fd[0]);
		close(in[1]);
		if (dup2(fd[1], STDOUT_FILENO) == -1) exit(EXIT_FAILURE);
		if (dup2(in[0], STDIN_FILENO) == -1) exit(EXIT_FAILURE);
		std::vector<char *> argv;
		argv.push_back(NULL);
		if (exec == "sh")
		{
			if (execvp(filepath.c_str(), &argv[0]) == -1)
			{
				std::cerr << "Error launching cgi: " << filepath << std::endl;
				exit(127);
			}
		} else
		{
			if (execlp(exec.c_str(), exec.c_str(), filepath.c_str(),
					   NULL) == -1)
			{
				std::cerr << "Error launching cgi: " << filepath << std::endl;
				exit(127);
			}
		}
	}
	close(fd[1]);
	close(in[0]);
	return (fd[0]);
}

int get_gci(std::string &buffer, const std::string &filepath, int in[2],
			const std::map<std::string, std::string> &meta_var,
			const int &bodySize)
{
	int fd;
	char *body = new char[bodySize]();

	fd = execute_cgi(filepath, in, meta_var);
	if (fd == -1) return (0);
	while (read(fd, body, bodySize) > 0) buffer += body;

	delete[] body;
	return (1);
}
