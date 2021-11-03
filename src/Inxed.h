#ifndef __INXED_H
#define __INXED_H

#ifndef _WIN32
#error Only windows supported
#endif

#include <stdio.h>
#include <string.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024
#define REQUEST_SIZE 64
#define IPV4_SIZE 16

#define IP_API_HOST "www.ip-api.com"
#define IP_API_ADDRESS "208.95.112.1"

int GenerateError(char *message);
int SocketCreate(SOCKET *st);
int SocketConnect(SOCKET *st, char *host, int port);
int SocketSend(SOCKET *st, char data[REQUEST_SIZE], char (*response)[BUFFER_SIZE]);
int GetIpAddressInformation(char *ip_address);

#endif
