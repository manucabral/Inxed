#include <stdio.h>
#include "src/Inxed.h"

int main(int argc, char *argv[])
{
    char ip[IPV4_SIZE];
    printf("Enter a IP: ");
    scanf("%s", &ip);
    GetIpAddressInformation(ip);
    return 0;
}