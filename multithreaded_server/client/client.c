/**
 * @date:   2022-11-22 10:22:43
 * @lastModifiedTime: 2022-11-22 11:36:51
 * @License: Apache License 2.0, https://www.apache.org/licenses/LICENSE-2.0
 */

#include "debugger.h"
#include <arpa/inet.h> //inet_addr
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int socket_desc;
    struct sockaddr_in server;
    int iPort = atoi("8080");
    char *message, server_reply[2000];
    size_t len = 0;
    ssize_t lineSize = 0;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        perror("no socket generated");
        printf("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(iPort);

    // Connect to remote server
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("connect error");
        return 1;
    }

    puts("Connected");
    if (recv(socket_desc, server_reply, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(server_reply);

    if (recv(socket_desc, server_reply, 2000, 0) < 0) {
        puts("recv failed");
    }
    puts(server_reply);
	memset(server_reply, 0, sizeof(server_reply));


    while ((lineSize = getline(&message, &len, stdin)) > 1) {

        if (send(socket_desc, message, strlen(message), 0) < 0) {
            puts("Send failed");
            free(message);
            close(socket_desc);
            return 1;
        }
        if (recv(socket_desc, server_reply, 2000, 0) < 0) {
            puts("recv failed");
        }
        puts(server_reply);
		memset(server_reply, 0, sizeof(server_reply));
        free(message);
        message = NULL;
    }
    close(socket_desc);

    return 0;
}