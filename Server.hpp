#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <poll.h>
# include <vector>
# include <map>


////->> a mettre dans Serveur.cpp
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

class Server {
	public:
		Server(int port, const std::string  &password);
		~Server();
		void run();

	private:
		Server(const Server &other);
		Server &operator=(const Server &other);

		void setupListeningSocket();
		void setNonBlocking(int fd);
		void acceptLoop();
		void handleClientRead(int fd);/////////////////
		void removeClient(int fd);/////////////////////
		bool isFatalPollEvent(short revents) const;////

		int _port;
		std::string _password;
		int _serverFd;

		std::vector<struct pollfd> _pfds;//////////////
		std::map<int, Client> _clients;////////////////
};

#endif
