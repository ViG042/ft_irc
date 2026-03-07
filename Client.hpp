#ifndef CLIENT_HPP
# define CLIENT_HPP

#define MAX_INBUFFER_SIZE  (1 << 13) //2^13 -> 8KB
#define MAX_OUTBUFFER_SIZE (1 << 16) //2^16 -> 64KB

# include <string>

struct Client {
	Client() : fd(-1) {}

	int fd;
	std::string inBuffer;
	std::string outBuffer;
};

#endif
