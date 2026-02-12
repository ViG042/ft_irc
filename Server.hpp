#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>

#include <iostream>
#include <cerrno>       //errno
#include <cstring>      //strerror, std::memset
#include <cstdlib>      //exit
#include <unistd.h>     //close
#include <fcntl.h>      //fcntl
#include <sys/socket.h> //socket, setsockopt, bind, listen
#include <netinet/in.h> //sockaddr_in, htons/l, INADDR_ANY
#include <arpa/inet.h>  //inet_ntoa,
#include <poll.h>       //poll

struct Client {
	int fd;
	std::string inBuffer;
	std::string outBuffer;
};

class Server {
public:
	Server(int port, const std::string& password);
	~Server();
	void run();

private:
	Server(const Server&);
	Server& operator=(const Server&);

	void setupListeningSocket();
	void setNonBlocking(int fd);
	void acceptLoop();

	int _port;
	std::string _password;
	int _serverFd;

	std::map<int, Client> _clients;
};

#endif
