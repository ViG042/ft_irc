#include "Server.hpp"
#include <iostream>
#include <cerrno>       //errno
#include <cstring>      //strerror, std::memset
#include <cstdlib>      //exit
#include <unistd.h>     //close
#include <fcntl.h>      //fcntl
#include <sys/socket.h> //socket, setsockopt, bind, listen
#include <netinet/in.h> //sockaddr_in, htons/l, INADDR_ANY

#define MAX_PENDING_CONNECTIONS 128

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
		die("fcntl(F_GETFL) failed");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		die("fcntl(F_SETFL) failed");
}

void Server::setupListeningSocket() {
	_serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_serverFd == -1)
		die("socket() failed");

	int optVal = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR,
			&optVal, sizeof(optVal)) == -1)
		die("setsockopt(SO_REUSEADDR) failed");

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(((unsigned short)_port));

	if (bind(_serverFd, (const sockaddr*)&addr, sizeof(addr)) == -1)
		die("bind() failed");

	if (listen(_serverFd, MAX_PENDING_CONNECTIONS) == -1)
		die("listen() failed");

	setNonBlocking(_serverFd);
}

void Server::acceptLoop() {}

void Server::run() {}
