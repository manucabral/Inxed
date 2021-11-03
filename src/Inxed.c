#include "Inxed.h"

int GenerateError(char *message)
{
    fprintf(stderr, "%s (%d)", message, WSAGetLastError());
    return EXIT_FAILURE;
}

int SocketSend(SOCKET *st, char data[REQUEST_SIZE], char (*response)[BUFFER_SIZE])
{
    int bytes_received;
    char buffer[BUFFER_SIZE];

    send(*st, data, REQUEST_SIZE, 0);

    if ((bytes_received = recv(*st, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR)
        return GenerateError("Error on receive");

    buffer[bytes_received] = '\0';
    strcpy(*response, buffer);
    return 0;
}

int SocketCreate(SOCKET *st)
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return GenerateError("Failed WSA startup");

    if ((*st = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        return GenerateError("Error on created the socket");
}

int SocketConnect(SOCKET *st, char *host, int port)
{
    struct sockaddr_in server;
    memset(&server, 0, sizeof server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_port = htons(port);
    if (connect(*st, (struct sockaddr *)&server, sizeof server) < 0)
        return GenerateError("Failed to connect");
    return 0;
}

int GetIpAddressInformation(char *ip_address)
{
    SOCKET socket;
    char request[REQUEST_SIZE], response[BUFFER_SIZE];
    snprintf(request, sizeof request, "GET /csv/%s HTTP/1.1\r\nHost: %s\r\n\r\n", ip_address, IP_API_HOST);
    SocketCreate(&socket);
    SocketConnect(&socket, IP_API_ADDRESS, 80);
    SocketSend(&socket, request, &response);
    closesocket(socket);
    WSACleanup();
    puts(response);
    return 0;
}