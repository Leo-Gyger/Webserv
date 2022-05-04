//
// Created by Mano Segransan on 3/7/22.
//

#include "parser_utils.hpp"
#include <Server.hpp>
#include <algorithm>
#include <parser.hpp>
#include <vector>

int parse_route_line(Route &r, t_file &f)
{
	if (f.line.find('}') == 0) return (1);
	if (f.line.find("default") == 0)
	{
		f.line.erase(0, 7);
		f.j += trim_left(f.line) + 7;
		r.setDefaultFile(f.line);
	} else if (f.line.find("path") == 0)
	{
		if (r.getCGI() || r.getRedir() || r.getPath())
			parse_error(f, "route cannot have more than one of path/redir/cgi");
		f.line.erase(0, 4);
		f.j += trim_left(f.line) + 4;
		r.setRoute(f.line);
		r.setPath(true);
	} else if (f.line.find("redir") == 0)
	{
		if (r.getCGI() || r.getRedir() || r.getPath())
			parse_error(f, "route cannot have more than one of path/redir/cgi");
		f.line.erase(0, 5);
		f.j += trim_left(f.line) + 5;
		r.setRoute(f.line);
		r.setRedir(true);
	} else if (f.line.find("cgi") == 0)
	{
		if (r.getCGI() || r.getRedir() || r.getPath())
			parse_error(f, "route cannot have more than one of path/redir/cgi");
		f.line.erase(0, 3);
		f.j += trim_left(f.line) + 3;
		r.setRoute(f.line);
		r.setCGI(true);
	} else if (f.line.find("url") == 0)
	{
		f.line.erase(0, 3);
		f.j += trim_left(f.line) + 3;
		r.setUrl(f.line);
	} else if (f.line.find("methods") == 0)
	{
		f.line.erase(0, 7);
		f.j += trim_left(f.line) + 7;
		std::string token;
		size_t pos;
		while (!f.line.empty())
		{
			pos = f.line.find(',');
			token = f.line.substr(0, pos);
			r.method.push_back(token);
			if (token == "GET") r.addMethods(GET);
			else if (token == "POST")
				r.addMethods(POST);
			else if (token == "DELETE")
				r.addMethods(DELETE);
			else if (token == "PUT")
				r.addMethods(PUT);
			else if (token == "HEAD")
				r.addMethods(HEAD);
			else
				parse_error(f, "unrecognized token: " + token);
			f.line.erase(0, pos + 1);
			f.j += pos + 1;
			if (pos == std::string::npos) break;
		}
	} else if (f.line.find("client_max_body_size") == 0)
	{
		f.line.erase(0, 20);
		f.j += trim_left(f.line) + 20;
		r.setMaxBodySize(ft_stoi(f.line));
	} else
		parse_error(f, "Unrecognized token in route directive");
	return (0);
}

Route parse_route(t_file &f)
{
	Route r;

	if (!f.line.empty())
	{
		if (f.line.find('}') == 0)
		{
			if (r.getCGI() || r.getRedir() || r.getPath()) return (r);
			parse_error(f, "missing directive in route");
		}
		parse_error(f, "garbage at EOL");
	}
	while (f.file->good() && !f.file->eof())
	{
		++(f.i);
		f.j = 1;
		std::getline(*f.file, f.line);
		remove_comments(f.line);
		f.j += trim_left(f.line);
		if (!f.line.empty())
			if (parse_route_line(r, f)) return (r);
	}
	parse_error(f, "missing `}` before EOF");
	return (r);
}

int parse_line(Server &sv, t_file &f)
{
	if (f.line.find('}') == 0) return (1);
	if (f.line.find("port") == 0)
	{
		f.line.erase(0, 4);
		f.j += trim_left(f.line) + 4;
		sv.setPort(ft_stoi(f.line));
		f.j += trim_left_number(f.line);
		if (!f.line.empty()) parse_error(f, "garbage at EOL");
	} else if (f.line.find("route") == 0)
	{
		f.line.erase(0, 5);
		f.j += trim_left(f.line) + 5;
		if (f.line.empty())
			if (skip_lines(f)) parse_error(f, "unexpected EOF");
		if (f.line.find('{') != 0) parse_error(f, "unrecognized token");
		f.line.erase(0, 1);
		f.j += trim_left(f.line) + 1;
		sv.addRoute(parse_route(f));
	} else if (f.line.find("body") == 0)
	{
		f.line.erase(0, 4);
		f.j += trim_left(f.line) + 4;
		sv.setBody(ft_stoi(f.line));
		f.j += trim_left_number(f.line);
		if (!f.line.empty()) parse_error(f, "garbage at EOL");
	} else if (f.line.find("server_name") == 0)
	{
		f.line.erase(0, 11);
		f.j += trim_left(f.line) + 11;
		sv.setServerName(f.line);
		f.j += trim_left_number(f.line);
		if (!f.line.empty()) parse_error(f, "garbage at EOL");
	} else
		parse_error(f, "Unrecognized token in server directive");
	return (0);
}

Server parse_server(t_file &f)
{
	Server sv;

	if (!f.line.empty())
	{
		if (f.line.find('}') == 0) return (sv);
		parse_error(f, "garbage at EOL");
	}
	while (f.file->good() && !f.file->eof())
	{
		++(f.i);
		f.j = 1;
		std::getline(*f.file, f.line);
		remove_comments(f.line);
		f.j += trim_left(f.line);
		if (!f.line.empty())
			if (parse_line(sv, f)) return (sv);
	}
	parse_error(f, "missing `}` before EOF");
	return (sv);
}

int get_server_directive(t_file &f)
{
	if (skip_lines(f)) { return (1); }
	if (f.line.find("server") != 0) parse_error(f, "unrecognized token");
	f.line.erase(0, 6);
	f.j += trim_left(f.line) + 6;
	if (f.line.empty())
		if (skip_lines(f)) parse_error(f, "unexpected EOF");
	if (f.line.find('{') != 0) parse_error(f, "unrecognized token");
	f.line.erase(0, 1);
	f.j += trim_left(f.line) + 1;
	return (0);
}

std::vector<Server> parse_config_file(const char *filename)
{
	t_file f;
	std::ifstream file(filename);
	f.filename = filename;
	f.file = &file;
	f.i = 0;
	f.j = 1;

	std::vector<Server> serverList;

	if (!f.file->is_open()) exit(EXIT_FAILURE);
	if (get_server_directive(f))
		parse_error(f, "could not find server directive");
	serverList.push_back(parse_server(f));
	while (!get_server_directive(f)) serverList.push_back(parse_server(f));
	f.file->close();
	return (serverList);
}
