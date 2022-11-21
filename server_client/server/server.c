#include "debugger.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int GetArgs(int *port, char **id, int argc, char *argv[]);
int GetTimeNow(char *zTime);
char *GetNameOfMonth(int month);
int getServer(int *server, int port);
int sendHeader(int socket, char *id);
int handshake(int socket, char *id);
int GetRecipient(int socket, char *recipient);
int GetSender(int socket, char *sender);
char *getData(int socket);
int SendClose(int socket);

int main(int argc, char *argv[]) {

    int socket_desc, socket, c;
    struct sockaddr_in client;
    char sender[100] = {0};
    char recipient[100] = {0};
    char *data = NULL;

    int port = 0;
    char *id = NULL;

    if (GetArgs(&port, &id, argc, argv) != 0) {
        return 1;
    }
    if (getServer(&socket_desc, port) != 0) {
        error("no socket generated");
        return 1;
    }

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
    puts("Connection accepted");

    if (handshake(socket, id) != 0) {
        error("handshake failed");
        close(socket);
        return 1;
    }

    if (GetSender(socket, sender) != 0) {
        error("get sender failed");
        close(socket);
        return 1;
    }
    if (GetRecipient(socket, recipient) != 0) {
        error("get recipient failed");
        close(socket);
        return 1;
    }
    data = getData(socket);
    puts(data);
    if (SendClose(socket) != 0) {
        error("failed to receive and/or send closing line");
        close(socket);
        return 1;
    }
    close(socket);
    free(data);
    data = NULL;
    return 0;
}


int SendClose(int socket){
    char buffer[2000]= {0};

     if (read(socket, buffer, 2000 - 1) < 0) {
        puts("recv failed");
        return 1;
    }
    puts(buffer);
    if (strncmp(buffer, "QUIT", 4) != 0) {
        puts("validation failed");
        return 1;
    }
    char *msg = "221 127.0.0.1 closing connection\r\n";
    if (write(socket, msg, strlen(msg)) < 0) {
        puts("write failed");
        return 1;
    }

    return 0;
};

char *getData(int socket) {
    char buffer[2000] = {0};
    char *message = NULL;
    int iMessage = 0;
    int iBytesRecv = 0;

    if (read(socket, buffer, 2000 - 1) < 0) {
        puts("recv failed");
        return NULL;
    }
    puts(buffer);
    if (strncmp(buffer, "DATA", 4) != 0) {
        puts("validation failed");
        return NULL;
    }
    char *msg = "354 Ready for message\r\n";
    if (write(socket, msg, strlen(msg)) < 0) {
        puts("write failed");
        return NULL;
    }

    do {
        memset(buffer, 0, 2000);
        iBytesRecv = read(socket, buffer, 2000 - 1);

        if (iBytesRecv < 0) {
            error("read failed with error code: %i", errno);
            break;
        }
        if (iBytesRecv > 0) {
            debug("Received %i bytes from client", iBytesRecv);
            char *pszTemp = message;
            message = (char *)malloc(iMessage + iBytesRecv + 1);
            if (message != NULL) {
                memset(message, 0, iMessage + iBytesRecv + 1);
                if (pszTemp != NULL) {
                    memcpy(message, pszTemp, iMessage);
                    free(pszTemp);
                    pszTemp = NULL;
                }
                memcpy(message + iMessage, buffer, iBytesRecv);
                iMessage += iBytesRecv;
                debug("Total received bytes: %i", iMessage);
                if ( strcmp(message+iMessage-5, "\r\n.\r\n") == 0 ) {
                    debug("found end of data, length: %i",iMessage);
                    break;
                }
            }
        }

    } while (true);

    msg = "250 Message accepted\r\n";
    if (write(socket, msg, strlen(msg)) < 0) {
        puts("write failed");
        return NULL;
    }
    return message;
}

int GetSender(int socket, char *sender) {

    if (read(socket, sender, 2000 - 1) < 0) {
        puts("recv failed");
        return 1;
    }
    puts(sender);
    if (strncmp(sender, "MAIL FROM:", 10) != 0) {
        puts("validation failed");
        return 1;
    }
    char *msg = "250 Sender address ok\r\n";
    if (write(socket, msg, strlen(msg)) < 0) {
        puts("write failed");
        return 1;
    }
    return 0;
}

int GetRecipient(int socket, char *recipient) {
    if (read(socket, recipient, 2000 - 1) < 0) {
        puts("recv failed");
        return 1;
    }
    puts(recipient);
    if (strncmp(recipient, "RCPT TO:", 8) != 0) {
        puts("validation failed");
        return 1;
    }
    char *msg = "250 Recipient address ok\r\n";
    if (write(socket, msg, strlen(msg)) < 0) {
        puts("write failed");
        return 1;
    }
    return 0;
}

int handshake(int socket, char *id) {
    char messageBuf[2000] = {0};
    char clientName[200] = {0};

    memset(messageBuf, 0, sizeof(messageBuf));

    if (sendHeader(socket, id) != 0) {
        error("sendHeader failed");
        return 1;
    }

    if (read(socket, messageBuf, 2000 - 1) < 0) {
        error("recv failed");
        puts("recv failed");
        return 1;
    }

    if (strncmp(messageBuf, "HELO", 4) != 0) {
        error("HELO not found");
        return 1;
    }
    strncpy(clientName, messageBuf + 4, (strchr(messageBuf, '.') - messageBuf - 4));
    memset(messageBuf, 0, sizeof(messageBuf));

    strcat(messageBuf, "250 127.0.0.1 Hello");
    strcat(messageBuf, clientName);
    puts(messageBuf);
    int res = write(socket, messageBuf, strlen(messageBuf));
    if (res < 0) {
        error("write failed");
        puts("write failed");
        return 1;
    }
    return 0;
}

int sendHeader(int socket, char *id) {
    char messageBuf[200] = {0};
    char time[25] = {0};
    if (GetTimeNow(time) != 0) {
        error("GetTimeNow failed");
        return 1;
    };
    strcat(messageBuf, "220 127.0.0.1 SMTP ");
    strcat(messageBuf, id);
    strcat(messageBuf, time);

    int res = write(socket, messageBuf, strlen(messageBuf));
    if (res < 0) {
        error("write failed");
        puts("write failed");
        return 1;
    }
    memset(messageBuf, 0, sizeof(messageBuf));
    puts("header sent");
    return 0;
}

int getServer(int *server, int port) {
    struct sockaddr_in serverConfig;
    *server = socket(AF_INET, SOCK_STREAM, 0);
    if (*server == -1) {
        error("no socket generated");
        puts("Could not create socket");
        return 1;
    }

    serverConfig.sin_addr.s_addr = INADDR_ANY;
    serverConfig.sin_family = AF_INET;
    serverConfig.sin_port = htons(port);

    if (bind(*server, (struct sockaddr *)&serverConfig, sizeof(serverConfig)) != 0) {
        error("bind failed");
        puts("bind failed");
        return 1;
    }
    puts("bind done");
    listen(*server, 5);
    return 0;
}

int GetTimeNow(char *zTime) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int res = sprintf(zTime, " %d %s %02d, %02d:%02d:%02d", tm.tm_mday, GetNameOfMonth(tm.tm_mon + 1),
                      tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    if (res < 0) {
        error("sprintf failed");
        return 1;
    }
    return 0;
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