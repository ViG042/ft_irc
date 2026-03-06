#include "Server.hpp"
#include <iostream>
#include <unistd.h> //close

void Server::addClient(int fd) {
	Client &client = _clients[fd];
	client.fd = fd;

	pollfd clientPollfd;
	clientPollfd.fd = fd;
	clientPollfd.events = POLLIN;
	clientPollfd.revents = 0;

	_pfds.push_back(clientPollfd);

	std::cout << "in ClientI: fd=" << fd << std::endl;
}

void Server::removeClient(int fd) {
	close(fd);

	_clients.erase(fd);

	for (size_t i = 0; i < _pfds.size(); ++i) {
		if (_pfds[i].fd == fd) {
			_pfds.erase(_pfds.begin() + i);
			break;
		}
	}

	std::cout << "in ClientO fd=" << fd << std::endl;
}
