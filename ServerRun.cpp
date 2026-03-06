#include "Server.hpp"
#include <stdexcept>
#include <iostream>
#include <cerrno>
#include <unistd.h> //close

Server::Server(int port, const std::string &password) :	_port(port), _password(password), _serverFd(-1) {}

Server::~Server() {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
	}
	if (_serverFd != -1)
		close(_serverFd);
}

void Server::run() {
	setupListeningSocket();
	initPollSet();

	while (true) {
		int pollReturn = poll(&_pfds[0], _pfds.size(), -1);
		if (pollReturn == -1) {
			if (errno == EINTR) {//interruption par un signal
				continue;
			}
			throw std::runtime_error("poll() failed");
		}
		size_t i = 0;
		while (i < _pfds.size()) {
			pollEvent(i);
		}
	}
}
