IRC = Internet Relay Chat

-----------------------------------------
#include <pollfd>

structpollfd {
  int fd;          //_serverFd
  short events;    //events a surveiller
  short revents;   //events survenus
};

POLLIN = pret a lire

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
fds = tableau de pollfd
nfds = nombre d'elements
timeout: -1 = indefiniment, 0 = ne pas attendre, >0 = X millisecondes
------------------------------------------

void Server::acceptLoop() {
	while (true) {
		sockaddr_in addr;
		socklen_t addrLen = sizeof(addr);
		int clientFd = accept(_serverFd, (sockaddr*)&addr, &addrLen);
		if (clientFd == -1)
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			die("accpet() failed");
		std::cout << "clientFD = " << clientFd << std::endl;
		std::cout << "client adress = " << inet_ntoa(addr.sin_addr) << std::endl;
		close(clientFd);
	}
