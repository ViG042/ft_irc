#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include <string>
# include <poll.h>
# include <vector>
# include <map>
# include <stdexcept>

class Server {
	public:
		Server(int port, const std::string  &password);
		~Server();
		void run();

	private:
		Server(const Server &other);
		Server &operator=(const Server &other);

		//ServerConf.cpp
		void initPollSet();
static std::runtime_error runtimeError(const std::string &explain);
		void setNonBlocking(int fd);
		void setupListeningSocket();
		//ClientAccept.cpp
		void clientAccept();
		//ClientAddRemove.cpp
		void addClient(int fd);
		void removeClient(int fd);
		//PollEvent.cpp
		bool isFatalPollEvent(short revents) const;
		void pollEvent(size_t &i);
		//ClientIO.cpp
		void clientRead(int fd);

		int _port;
		std::string _password;
		int _serverFd;
		std::vector<struct pollfd> _pfds;
		std::map<int, Client> _clients;
};

#endif
