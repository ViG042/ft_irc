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
		static std::runtime_error runtimeError(const std::string&);
		void setNonBlocking(int fd);
		void setupListeningSocket();
		//ClientAddRemove.cpp
		void clientAccept();
		void clientAdd(int fd);
		void clientRemove(int fd);
		//PollEvent.cpp
		void updatePollEvents(int fd);
		bool isFatalPollEvent(short revents) const;
		void pollEvent(size_t &i);
		//ClientIO.cpp
		void clientIO_error(const std::string &message, int fd);
		void queueMessage(int fd, const std::string &message);
		void clientRead(int fd);
		void clientWrite(int fd);
		//ClientGetters.cpp
		Client &getClient(int fd);
		pollfd &getPollFd(int fd);

		int _port;
		std::string _password;
		int _serverFd;
		std::vector<struct pollfd> _pfds;
		std::map<int, Client> _clients;
};

#endif
