#include "Server.hpp"
#include <stdexcept> //std::runtime_error
#include <poll.h>    //POLLERR, POLLHUP, POLLVAL, POLLIN

bool Server::isFatalPollEvent(short revents) const {
	return ((revents & (POLLERR | POLLHUP | POLLNVAL)) != 0);
}

void Server::pollEvent(size_t &i) {
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
			clientAccept();
			++i;
			return;
		}
	}

	if (isFatalPollEvent(revents)) {
		removeClient(currentFd);
		return;
	}

	if (revents & POLLIN) {
		clientRead(currentFd);
		if (_clients.find(currentFd) != _clients.end()) {
			++i;
		}
		return;
	}

	++i;
}
