#include "debugger.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void *connection_handler(void *);

int main(int argc, char *argv[]) {
    int socket_desc, new_socket, c, *new_sock;
    struct sockaddr_in server, client;
    char *message;

    int iPort = atoi("8080");

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        error("no socket generated");
        puts("Could not create socket");
        return 1;
    }
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(iPort);

    // Connect to remote server
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        error("bind failed");
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    listen(socket_desc, 5);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
        pthread_t sniffer_thread;
        puts("Connection accepted");
        message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
        if (write(new_socket, message, strlen(message)) < 0) {
            error("write failed");
            close(socket_desc);
            return 1;
        }

        new_sock = malloc(1);
        *new_sock = new_socket;
        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0) {
            error("could not create thread");
            close(socket_desc);
            return 1;
        }
        puts("Handler assigned");
    }

    if (new_socket < 0) {
        error("accept failed");
        close(socket_desc);
        return 1;
    }

    close(socket_desc);
    return 0;
}

/*
 * This will handle connection for each cli	ent
 *
 */
void *connection_handler(void *socket_desc) {
    int sock = *(int *)socket_desc;
    int read_size;
    char *message, client_message[2000];

    message = "Greetings! I am your connection handler\n Now type something and i shall repeat what you type \n";
    if (write(sock, message, strlen(message)) < 0) {
        error("write failed");
        return (void*)1;
    };

    while ((read_size = read(sock, client_message, 2000)) > 0) {
        printf("%s", client_message);
        // Send the message back to client
        if (write(sock, client_message, strlen(client_message)) < 0) {
            puts("write failed");
        }
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        error("recv failed");
        perror("recv failed");
    }

    free(socket_desc);
    return (void*)0;
}
