#include "Server.hpp"
#include <iostream>
#include <stdexcept>    //std::runtime_error
#include <cerrno>       //errno
#include <cstring>      //std::strerror
#include <sys/socket.h> //recv

void Server::clientRead(int fd) {
	char buffer[512];
	ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);

	if (bytesRead == 0) {
		removeClient(fd);
		return;
	}
	if (bytesRead == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return;
		}
		std::cout << "recv() error: " << std::strerror(errno)
				  << ", closing client fd=" << fd << std::endl;
		removeClient(fd);
		return;
	}
	std::map<int, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end()) {
		throw std::runtime_error("can't find fd in client map ");
	}
	it->second.inBuffer.append(buffer, (size_t)bytesRead);

int buffSize = it->second.inBuffer.size();///////////////
std::cout << "---> in clientIO:" << std::endl;///////////
std::cout << "From fd=" << fd << std::endl;//////////////
std::cout << "Bytes read=" << bytesRead << std::endl;////
std::cout << "inBuffer size=" << buffSize << std::endl;//
std::cout << "buffer: " << buffer << std::endl;//////////
}
