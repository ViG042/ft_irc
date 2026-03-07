#include "Server.hpp"

Client &Server::getClient(int fd) {
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end()) {
		throw std::runtime_error("can't find fd in _clients map");
	}
	return it->second;
}

pollfd &Server::getPollFd(int fd) {
	for (size_t i = 0; i < _pfds.size(); ++i) {
		if (_pfds[i].fd == fd) {
			return _pfds[i];
		}
	}
	throw std::runtime_error("can't find fd in _pfds vector");
}
