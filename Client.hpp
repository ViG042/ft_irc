#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

struct Client {
	Client() : fd(-1) {}

	int fd;
	std::string inBuffer;
	std::string outBuffer;
};

#endif
