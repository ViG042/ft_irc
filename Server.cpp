#include "Server.hpp"
#include <iostream>
#include <cerrno>     //errno
#include <cstring>    //strerror
#include <cstdlib>    //exit
#include <unistd.h>   //close
#include <fcntl.h>    //fcntl

static void die(const std::string& msg) {
	std::cerr << "Error: " << msg;
	if (errno != 0)
		std::cerr << " ()" << std::strerror(errno) << ")";
	std::exit(1);
}

Server::Server(int port, const std::string& password)
: _port(port), _password(password), _serverFd(-1) {}

Server::~Server() {
	if (_serverFd != -1)
		close(_serverFd);
}

void Server::setNonBlocking(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		die("fcntl(fd, F_GETFL, 0) failed");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		die("fcntl(fd, F_SETFL, flags | O_NONBLOCK) failed");
}

void Server::setupListeningSocket() {}

void Server::acceptLoop() {}

void Server::run() {}
