#include "Server.hpp"
#include <string>
#include <stdexcept>    //std::runtime_error
#include <cerrno>       //errno
#include <cstring>      //std::strerror
#include <sys/socket.h> //accept, sockaddr
#include <netinet/in.h> //sockaddr_in
#include <iostream>

void Server::clientAccept() {
	while (true) {
		sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		int clientFd = accept(_serverFd, (sockaddr *)&clientAddr, &clientAddrLen);
		if (clientFd == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;   //il n'y a plus de connections en attente
			}
			throw Server::runtimeError("accept() failed");
		}
		setNonBlocking(clientFd);
		addClient(clientFd);

		std::cout << "in ClientAccept.cpp: fd="
				  << clientFd << std::endl << std::endl;
	}
}
