#include "debugger.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(void) {

    char *HostAddress = "eastwillsecurity.com";
    char resourcePath[] = "pg3401/test.html";
    char request[101] = {0};

    struct hostent *hp;
    int socket_desc;
    struct sockaddr_in client;
    int iPort = atoi("80");

    char buf[1500] = {0};
    int byte_count = 0;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        perror("no socket generated");
        printf("Could not create socket");
        return 1;
    }

    hp = gethostbyname(HostAddress);
    if (hp == NULL) {
        printf("unknown host %d",h_errno );
        return 1;
    }

    memcpy((char *)&client.sin_addr.s_addr, (char *)hp->h_addr_list[0], hp->h_length);
    client.sin_family = AF_INET;
    client.sin_port = htons(iPort);

    // Connect to remote server
    if (connect(socket_desc, (struct sockaddr *)&client, sizeof(client)) < 0) {
        puts("connect error");
        return 1;
    }
    puts("Connected");

    sprintf(request, "GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", resourcePath, HostAddress);

    //send request to server
    send(socket_desc, request, strlen(request), 0);

    //Receive a reply from the server
    if ((byte_count = recv(socket_desc, buf, 1500, 0)) < 0) {
        puts("recv failed");
    }    
    byte_count = recv(socket_desc, buf, sizeof(buf) - 1, MSG_WAITALL); 

    buf[byte_count] = 0;
    printf("%s",buf);
    close(socket_desc);
}