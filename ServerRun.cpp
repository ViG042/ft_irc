#include "Server.hpp"

#include <stdexcept>
#include <iostream>
#include <cerrno>

#include <unistd.h> //close


Server::Server(int port, const std::string &password) :	_port(port), _password(password), _serverFd(-1) {}

Server::~Server() {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _client.end(); ++it)
	{
		close(it->first)
	}
	if (_serverFd != -1)
		close(_serverFd):
}



void Server::initPollSet() {
	_pfds.clear();

	pollfd serverPollifd;
	serverPollfd.events = POLLIN;
	serverPollfd.revents = 0;

	_pfds.push_back(serverPollfd);
}

void Server::run() {
	setupListeningSocket();
	initPollSet();

	while (true) {
		int pollReturn = poll(&_pfds[0], _pfds.size(), -1);
		if (pollReturn == -1) {
			if (errno == EINTR) //interruption par un signal
				continue;
		throw std::runtime_error("poll() failed");
		}
		size_t i = 0;
		while (i < _pfds.size()) {
				handlePollEvent(i);
			}
		}
	}
}

void Server::handlePollEvent(size_t &i) {
	const int currentFd = _pfds[i].fd;
	const short revents = _pfds[i].revents;

	if (revents == 0) {
		++i;
		return;
	}

	if (currentFd == _serverFd) {
		if (isFatalPollEvent(revents))
			throw std::runtime_error("Fatal poll event on server");
		if (revents & POLLIN) {
			acceptLoop();
		++i;
		return;
		}
	}

	if (isFatalPollEvent(revents)) {
		removeClient(currentFd);
		return;
	}

	if (revents & POLLIN) {
		handleClientRead(currentFd);
		if (_clients.find(currentFd) != _clients.end()) {
			++i;
		}
		return;
	}

	++i;
}












































