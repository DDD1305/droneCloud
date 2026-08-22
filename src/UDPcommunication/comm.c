#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>


int status; //socket indentification 
struct addrinfo hints;
struct addrinfo *servinfo;

hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_DGRAM;



