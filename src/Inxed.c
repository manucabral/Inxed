#include "Inxed.h"

const char fields[MAX_FIELD_QUERY][20] =
    {"Continent",
     "Country",
     "Country Code",
     "Region",
     "Region Name",
     "City",
     "Zip Code",
     "Latitude",
     "Longitude",
     "Timezone",
     "ISP Name",
     "Organization Name",
     "As Number",
     "Reverse DNS",
     "Proxy",
     "Ip Address"};

int GenerateError(char *message)
{
    fprintf(stderr, "%s (%d)", message, WSAGetLastError());
    return EXIT_FAILURE;
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

int PrintInformation(char *response)
{
    char *token, *parsed = strtok(response, "\n");
    int option = 0;

    for (size_t i = 0; i < 8; i++)
        parsed = strtok(NULL, "\n");

    token = strtok(parsed, ",");

    if (strcmp(token, "fail") == 0)
    {
        char error[64];
        token = strtok(NULL, ",");
        if (strcmp(token, "private range") == 0)
        {
            token = strtok(NULL, ",");
            snprintf(error, sizeof error, "Error: %s is a private IP address", token);
        }
        else if (strcmp(token, "reversed range") == 0)
        {
            token = strtok(NULL, ",");
            snprintf(error, sizeof error, "Error: %s is a reversed IP address", token);
        }
        else
            snprintf(error, sizeof error, "Error: invalid query", token);
        puts(error);
        return 1;
    }
    else
    {
        token = strtok(NULL, ",");
        printf("\n");
        for (size_t i = 0; i < MAX_FIELD_QUERY; i++)
        {
            printf("> %s: \%s\n", fields[i], token);
            token = strtok(NULL, ",");
        }
        return 0;
    }
}

int GenerateNewRequest(char (*req)[REQUEST_SIZE], char *ip_address)
{
    snprintf(*req, REQUEST_SIZE, "GET /csv/%s?fields=status,message,continent,country,countryCode,region,regionName,city,zip,lat,lon,timezone,isp,org,as,reverse,proxy,query HTTP/1.1\r\nHost: %s\r\n\r\n", ip_address, IP_API_HOST);
    return 0;
}

int GetIpAddressInformation(char *ip_address)
{
    SOCKET socket;
    char request[REQUEST_SIZE], response[BUFFER_SIZE];
    GenerateNewRequest(&request, ip_address);
    SocketCreate(&socket);
    SocketConnect(&socket, IP_API_ADDRESS, 80);
    SocketSend(&socket, request, &response);
    closesocket(socket);
    WSACleanup();
    return PrintInformation(response);
}