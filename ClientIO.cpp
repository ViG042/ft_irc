#include "Server.hpp"
#include <iostream>
#include <cerrno>       //errno
#include <cstring>      //std::strerror
#include <sys/socket.h> //recv

void Server::clientIO_error(const std::string &mess, int fd) {
	std::cout << "Error in clientIO.cpp: " << mess
			  << ", closing client fd=" << fd << std::endl;
	clientRemove(fd);
}

void Server::queueMessage(int fd, const std::string &message) {
	Client &client = getClient(fd);
	client.outBuffer.append(message);

	if (client.outBuffer.size() > MAX_OUTBUFFER_SIZE) {
		clientIO_error("outBuffer Overflow", fd);
		return;
	}

	updatePollEvents(fd);
}

void Server::clientRead(int fd) {
	char buffer[512];
	ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);
	//deconnexion normale  du client
	if (bytesRead == 0) {
		clientRemove(fd);
		return;
	}
	//signal a passer OU erreur recv()
	if (bytesRead == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return;
		}
		clientIO_error("recv() fail", fd);
		return;
	}
	//clientRead = ecrire dans inBuffer
	Client &client = getClient(fd);
	client.inBuffer.append(buffer, (size_t)bytesRead);

	if (client.inBuffer.size() > MAX_INBUFFER_SIZE) {
		clientIO_error("inBuffer overflow", fd);
		return;
	}
	//juste du debug qu'on pourra retirer a la fin du projet///////
	int buffSize = client.inBuffer.size();/////////////////////////
	std::cout << "in clientIO: clientRead" << std::endl;///////////
	std::cout << "From fd=" << fd << std::endl;////////////////////
	std::cout << "Bytes read=" << bytesRead << std::endl;//////////
	std::cout << "inBuffer size=" << buffSize << std::endl;////////
	std::cout << "inBuffer: " << client.inBuffer << std::endl;/////
	//*******       TEST ECHO     **********************8//////////
	queueMessage(fd, client.inBuffer);/////////////////////////////
	client.inBuffer.clear();///////////////////////////////////////
	//Leonard remplace le TEST ECHO par l'appel au parser//////////
	//Adrien utilise la fonction queueMessage(fd, message)/////////
}

void Server::clientWrite(int fd) {
	Client &client = getClient(fd);
	//finalement rien a ecrire
	if (client.outBuffer.empty()) {
		updatePollEvents(fd);
		return;
	}
	ssize_t bytesSent = send(fd, client.outBuffer.c_str(),
			client.outBuffer.size() ,0);
	//signal a passer OU erreur send()
	if (bytesSent == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return;
		}
		clientIO_error("send() fail", fd);
		return;
	}
	//gestion envoi nul, partiel ou complet
	client.outBuffer.erase(0, (size_t)bytesSent);
	updatePollEvents(fd);
	//juste du debug qu'on pourra retirer a la fin du projet//////
	int buffSize = client.outBuffer.size();///////////////////////
	std::cout << "in clientIO: clientWrite" << std::endl;/////////
	std::cout << "From fd=" << fd << std::endl;///////////////////
	std::cout << "Bytes sent=" << bytesSent << std::endl;/////////
	std::cout << "outBuffer size=" << buffSize << std::endl;//////
	std::cout << "outBuffer: " << client.outBuffer << std::endl;//
	std::cout << std::endl;///////////////////////////////////////
}
