#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <signal.h>

static int parsePort(const char *s) {
	std::istringstream iss(s);
	int port = 0;
	if (!(iss >> port) || !(iss.eof()))
		throw std::runtime_error("Port must be an integer");
	const int MAX_TCP_PORT = (1 << 16) - 1;
	if (port < 1 || port > MAX_TCP_PORT)
		throw std::runtime_error("Port range is 1-65535");
	return port;
}

static void parseArgs(int argc, char **argv,
					  int &port, std::string & password) {
	if (argc != 3) 
		throw std::runtime_error("Use: ./ircserv <port> <password>");
	port = parsePort(argv[1]);
	password = argv[2];
	if (password.empty())
		throw std::runtime_error("Password must not be empty");
	}

int main(int argc, char **argv) {
	//Ignorer SIGPIPE pour eviter que send() sur une socket fermee
	//termine le programme
	signal(SIGPIPE, SIG_IGN);
	try {
	int port;
	std::string password;
	parseArgs(argc, argv, port, password);

	Server server(port, password);
	server.run();
	}
	catch (const std::exception &e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	}
}
