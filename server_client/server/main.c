#include "debugger.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int GetArgs(int *port, char **id, int argc, char *argv[]);
char *GetTimeNow();
char *GetNameOfMonth(int month);

int main(int argc, char *argv[]) {

    int socket_desc, new_socket, c  ;
    struct sockaddr_in server, client;
    char *message;

    int port = 0;
    char *id = NULL;

    if (GetArgs(&port, &id, argc, argv) != 0) {
        return 1;
    }
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        error("no socket generated");
        puts("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to remote server
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) != 0) {
        error("bind failed");
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    listen(socket_desc, 5);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

    puts("Connection accepted");
    message = strcat(strcat("220 127.0.0.1 SMTP ",id),GetTimeNow());
    int res = write(new_socket , message , strlen(message));
    if(res < 0){
        error("write failed");
        puts("write failed");
        return 1;
    }
    puts("write done");

    return 0;
}

char *GetTimeNow() {
    char *strBuf = malloc(sizeof(char) * 25);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int res = sprintf(strBuf, "%d %s %02d, %02d:%02d:%02d", tm.tm_mday, GetNameOfMonth(tm.tm_mon + 1),
                      tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    if (res < 0) {
        error("sprintf failed");
        return NULL;
    }
    return strBuf;
}

char *GetNameOfMonth(int month) {
    char *months[] = {"jan", "Feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
    return months[month];
}

int GetArgs(int *port, char **id, int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-port") == 0) {
            if (i + 1 >= argc) {
                printf("missing value after -port\n");
                return 1;
            }
            *port = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-id") == 0) {
            if (i + 1 >= argc) {
                printf("missing value after -id\n");
                return 1;
            }
            *id = argv[i + 1];
        }
    }

    if (*port == 0 && *id == NULL) {
        printf("missing both flags -id and -port\n");
        return 1;
    }
    if (*port == 0) {
        printf("missing flag -port\n");
        return 1;
    }
    if (*id == NULL) {
        printf("missing flag -id\n");
        return 1;
    }
    return 0;
}