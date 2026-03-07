#include "Server.hpp"
#include <stdexcept> //std::runtime_error

bool Server::isFatalPollEvent(short revents) const {
	return ((revents & (POLLERR | POLLHUP | POLLNVAL)) != 0);
}

void Server::updatePollEvents(int fd) {
	Client &client = getClient(fd);

	short updatedEvents = POLLIN;
	if (!(client.outBuffer.empty())) {
		updatedEvents |= POLLOUT;
	}

	getPollFd(fd).events = updatedEvents;
}

void Server::pollEvent(size_t &i) {
	const int currentFd = _pfds[i].fd;
	const short revents = _pfds[i].revents;
	//finalement rien a faire
	if (revents == 0) {
		++i;
		return;
	}
	//evenement sur le serveur
	if (currentFd == _serverFd) {
		if (isFatalPollEvent(revents))
			throw std::runtime_error("Fatal poll event on server");
		if (revents & POLLIN) {
			clientAccept();
			++i;
			return;
		}
	}
	//evenement sur un client
	if (isFatalPollEvent(revents)) {
		clientRemove(currentFd);
		return;
	}
	if (revents & POLLIN) {
		clientRead(currentFd);
		if (_clients.find(currentFd) == _clients.end()) {
			return;
		}
	}
	if (revents & POLLOUT) {
		clientWrite(currentFd);
		if (_clients.find(currentFd) == _clients.end()) {
			return;
		}
	}
	++i;
}
