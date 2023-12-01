// tcp.c

#include "tcp.h"

int socketServer;

void sig_handler(int sig) {
    if (sig == SIGINT) {
        close(socketServer);
        printf("[Info] Close socket listening.\n");
        exit(0);
    }
}

int tcp() {
    struct sockaddr_in addrServer;
    int socketClient;
    struct sockaddr_in addrClient;

    socklen_t addrlen = sizeof(struct sockaddr);
    char recvBuf[RECV_BUFFER_LENGTH];
    char *sendBuf;

    socketServer = socket(AF_INET, SOCK_STREAM, 0);
    addrServer.sin_addr.s_addr = INADDR_ANY;
    addrServer.sin_family = AF_INET; // IPv4
    addrServer.sin_port = htons(80);
    bind(socketServer, (struct sockaddr *)&addrServer, sizeof(struct sockaddr));

    if (listen(socketServer, BACKLOG_LENGTH) == 0) {
        printf("[Info] Start to listen 0.0.0.0:80\n");
        signal(SIGINT, sig_handler);
        while (1) {
            socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &addrlen);
            if (socketClient != -1) {
                recv(socketClient, recvBuf, RECV_BUFFER_LENGTH, 0);
                sendBuf = http_process(recvBuf);
                if (sendBuf) {
                    send(socketClient, sendBuf, strlen(sendBuf), 0);
                }
                close(socketClient);
            }
        }
    } else {
        printf("[Error] Cannot listen.\n");
    }
    return 0;
}

