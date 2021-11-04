#include <stdio.h>
#include "src/Inxed.h"

int main(int argc, char *argv[])
{
    char ip[IPV4_SIZE], title[MAX_PATH], option;

    snprintf(title, sizeof title, "INXED %s", CURRENT_VERSION);
    SetConsoleTitle(title);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    while (option != 2)
    {
        system("cls");
        SetConsoleTextAttribute(hConsole, 12 | FOREGROUND_INTENSITY);
        printf("\n\t< I N X E D>\n");
        
        SetConsoleTextAttribute(hConsole, 4);
        printf("\t  by ne0de\n\n");

        SetConsoleTextAttribute(hConsole, 7);
        printf(">> Enter a IP: ");

        SetConsoleTextAttribute(hConsole, 2);
        scanf("%s", &ip);
        GetIpAddressInformation(ip);

        SetConsoleTextAttribute(hConsole, 7);
        printf("\n1: New query\n2: Exit\n");
        scanf("%d", &option);
    }

    SetConsoleTextAttribute(hConsole, saved_attributes);

    return 0;
}