#include "Server.hpp"
#include <cerrno>       //errno, EAGAIN, EWOULDBLOCK
#include <sys/socket.h> //accept, sockaddr
#include <iostream>
#include <netinet/in.h> //sockaddr_in
#include <unistd.h>     //close

void Server::clientAccept() {
	while (true) {
		sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		int clientFd = accept(_serverFd, (sockaddr *)&clientAddr, &    clientAddrLen);
		if (clientFd == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break; //il n'y a plus de connections en attente
			}
			throw Server::runtimeError("accept() failed");
		}
		setNonBlocking(clientFd);
		clientAdd(clientFd);
	}
}

void Server::clientAdd(int fd) {
	Client &client = _clients[fd];
	client.fd = fd;

	pollfd clientPollfd;
	clientPollfd.fd = fd;
	clientPollfd.events = POLLIN;
	clientPollfd.revents = 0;

	_pfds.push_back(clientPollfd);

	std::cout << "in ClientAddRemove.cpp: "
			  << "Coucou je suis le nouveau client fd="
			  << fd << std::endl << std::endl;
}

void Server::clientRemove(int fd) {
	close(fd);

	_clients.erase(fd);

	for (size_t i = 0; i < _pfds.size(); ++i) {
		if (_pfds[i].fd == fd) {
			_pfds.erase(_pfds.begin() + i);
			break;
		}
	}

	std::cout << "in ClientAddRemove.cpp: "
			  << "Je vous emmerde et je rentre a la maison fd="
			  << fd << std::endl << std::endl;
}
