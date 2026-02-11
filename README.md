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
