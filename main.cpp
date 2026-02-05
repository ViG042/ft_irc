#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <string>

static bool parsePort(const char* s, int &port) {
	std::istringstream iss(s);
	int p = 0;
	if (!(iss >> p))
		return false;
	if (!iss.eof())
		return false;
	if (p < 1 || p > 65535)
		return false;
	port = p;
	return true;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int port = 0;
	if (!parsePort(argv[1], port)) {
		std::cerr << "Invalid port (use 1 to 65535)" << std::endl;
		return 1;
	}

	std::string password = argv[2];
	if (password.empty()) {
		std::cerr << "Password must not be empty" << std::endl;
		return 1;
	}

	Server server(port, password);
	server.run();
	return 0;
}
