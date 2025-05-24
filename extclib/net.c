#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifdef __linux__
#include <unistd.h>
#include <arpa/inet.h>

#elif __WIN32
#include <winsock2.h>

#else
#warning "net.h: platform not supported"
#endif

#if defined(__linux__) || defined(__WIN32)


#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>


typedef enum error_t {
	WINSOCK_ERR = -1,
	SOCKET_ERR  = -2,
	SETOPT_ERR  = -3,
	PARSE_ERR   = -4,
	BIND_ERR    = -5,
	LISTEN_ERR  = -6,
	CONNECT_ERR = -7,
} error_t;

#include "net.h"


static int8_t _parse_address(char *address, char *ipv4, char *port);


int create_tcp_socket(char *address, struct sockaddr_in *addr){
#ifdef __WIN32
	WSDATA wsa;
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0){
		return WINSOCK_ERR;
	}
#endif	
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return SOCKET_ERR;
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
		return SETOPT_ERR;
	}
	char ipv4[16];
	char port[6];
	if (_parse_address(address, ipv4, port) != 0){
		return PARSE_ERR;
	}

	addr->sin_family = AF_INET;
	addr->sin_port = htons(atoi(port));
	addr->sin_addr.s_addr = inet_addr(ipv4);
	return sock;
}

extern int listen_net(char *address) {
	int listener;
	struct sockaddr_in addr;
	if ((listener = create_tcp_socket(address, &addr)) < 0 ){
		return listener; //return same error that create_socket returned 
	}
	if (bind(listener, (struct sockaddr*)&addr,   sizeof(addr)) != 0) {
		return BIND_ERR; 
	}

	if (listen(listener, SOMAXCONN) != 0){
		return LISTEN_ERR;
	}
	return listener;
}

extern int accept_net(int listener) {
	return accept(listener, NULL, NULL);
}

extern int connect_net(char *address) {
	int conn;
	struct sockaddr_in addr;
	if ((conn = create_tcp_socket(address, &addr)) < 0){
		return conn;
	}
	if (connect(conn, (struct sockaddr*)&addr, sizeof(addr)) != 0){
		return CONNECT_ERR;
	}
	return conn;
}
extern int close_net(int conn) {
#ifdef __linux__
	return close(conn);
#elif __WIN32
	return closesocket(conn);
#endif
}

extern int send_net(int conn, char *buffer, size_t size) {
	return send(conn, buffer, (int)size, 0);

}
extern int recv_net(int conn, char *buffer, size_t size) {
	return recv(conn, buffer, (int)size, 0);
}

static int8_t _parse_address(char *address, char *ipv4, char *port) {
	size_t i = 0, j = 0;
	for (; address[i] != ':'; ++i){
		if (address[i] == '\0'){
			return 1;
		}
		if (i >= 15){
			return 2;
		}
		ipv4[i] = address[i];
	}
	ipv4[i] = '\0';
	
	for (i += 1; address[i] != '\0'; ++i, ++j) {
		if (j >= 5) {
			return 3;
		}
		port[j] = address[i];
	}
	port[j] = '\0';
	return 0;
}

#endif
