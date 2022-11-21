#include "debugger.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int GetArgs(int *port, int argc, char *argv[]);

int main(int argc, char *argv[]) {

    int iSocket;
    struct sockaddr_in server;
    char strBuf[2000];

    int port = -1;

    if (GetArgs(&port, argc, argv) != 0) {
        return 1;
    }
    iSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (iSocket == -1) {
        error("no socket generated");
        puts("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    // Connect to remote server
    if (connect(iSocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("connect error");
        return 1;
    }

    puts("Connected");

    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));

    strcat(strBuf, "HELO bengt.127.0.0.1\r\n");
    if (send(iSocket, strBuf, strlen(strBuf), 0) < 0) {
        error("write failed");
        puts("Send failed");
        return 1;
    }
    puts("sent HELO");
    memset(strBuf, 0, sizeof(strBuf));

    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));

    //send mail from
    strcat(strBuf, "MAIL FROM: <bengt@test.com>\r\n");
    puts(strBuf);
    if (send(iSocket, strBuf, strlen(strBuf), 0) < 0) {
        error("write failed");
        puts("Send failed");
        return 1;
    }
    memset(strBuf, 0, sizeof(strBuf));

    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));
    //send rcpt to
    strcat(strBuf, "RCPT TO: <student@kristiania.no>\r\n");
    if (send(iSocket, strBuf, strlen(strBuf), 0) < 0) {
        error("write failed");
        puts("Send failed");
        return 1;
    }
    memset(strBuf, 0, sizeof(strBuf));
    
    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));

    strcat(strBuf, "DATA\r\n");
    if (send(iSocket, strBuf, strlen(strBuf), 0) < 0) {
        error("write failed");
        puts("Send failed");
        return 1;
    }
    memset(strBuf, 0, sizeof(strBuf));

    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));
    
    strcat(strBuf, "Test\r\n\r\n.\r\n");
    if (send(iSocket, strBuf, strlen(strBuf), 0) < 0) {
        error("write failed");
        puts("Send failed");
        return 1;
    }
    memset(strBuf, 0, sizeof(strBuf));

    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));

    strcat(strBuf, "QUIT\r\n");
    if (send(iSocket, strBuf, strlen(strBuf), 0) < 0) {
        error("write failed");
        puts("Send failed");
        return 1;
    }
    memset(strBuf, 0, sizeof(strBuf));

    if (recv(iSocket, strBuf, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(strBuf);
    memset(strBuf, 0, sizeof(strBuf));

    close(iSocket);
    return 0;
}

int GetArgs(int *port, int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-server") == 0) {
            if (i + 1 >= argc) {
                printf("missing value after -server\n");
                return 1;
            }
            *port = atoi(argv[i + 1]);
        }
    }
    if (*port == -1) {
        printf("missing flag -port\n");
        return 1;
    }
    return 0;
}