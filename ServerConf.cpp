#include "Server.hpp"
#include <cerrno>       //errno
#include <cstring>      //std::strerror, std::memset
#include <fcntl.h>      //fcntl, O_NONBLOCK
#include <sys/socket.h> //socket, setsockopt, bind, listen
#include <netinet/in.h> //sockaddr_in, htons, htonl, INADDR_ANY

#ifndef MAX_PENDING_CONNECTIONS
# define MAX_PENDING_CONNECTIONS 128
#endif

std::runtime_error Server::runtimeError(const std::string &mess) {
	return std::runtime_error(mess + ": " + std::strerror(errno));
}

void Server::initPollSet() {
	_pfds.clear();

	pollfd serverPollfd;
	serverPollfd.fd = _serverFd;
	serverPollfd.events = POLLIN;
	serverPollfd.revents = 0;

	_pfds.push_back(serverPollfd);
}

static sockaddr_in buildListenAddrIPv4(int port) {
	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET; //IPv4
	addr.sin_port = htons((unsigned short)port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	return addr;
}

void Server::setNonBlocking(int fd) {
	if(fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
		throw Server::runtimeError("fcntl(F_SETFL) failed");
	}
}

void Server::setupListeningSocket() {
	_serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_serverFd == -1)
		throw runtimeError("socket() failed");

	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw runtimeError("setsockopt(SO_REUSEADDR) failed");

	sockaddr_in addr = buildListenAddrIPv4(_port);


	if (bind(_serverFd, (const sockaddr *)&addr, sizeof(addr)) == -1)
		throw runtimeError("bind() failed");

	if (listen(_serverFd, MAX_PENDING_CONNECTIONS) == -1)
		throw runtimeError("listen() failed");
	//set non blocking
	setNonBlocking(_serverFd);
}
