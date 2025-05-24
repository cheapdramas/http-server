#ifndef EXTCLIB_HTTP_H
#define EXTCLIB_HTTP_H

#include <stdint.h>
#include <stddef.h>


typedef struct HTTPreq {
	char method[16]; // GET 
	char path[2048]; // /books
	char proto[16];  // HTTP/1.1
	uint8_t state;
	size_t index;
} HTTPreq;
typedef struct HTTP HTTP;

//extern because this function is going to be implemented in http.c
extern HTTP *new_http(char *address);
extern void *free_http(HTTP *http);

extern void handle_http(HTTP *http, char *path, void(*)(int, HTTPreq*));
extern int8_t listen_http(HTTP *http);

extern void parsehtml_http(int conn, char *filename);

#endif
