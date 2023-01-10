/*
 * udp chat room
 *
 * by Arthur Chang
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>

void *recvFuncThread(void *sock_fd);




typedef struct packetInfo{
    char userName[20];
    char msg[64];
    int  format;
}packetInfo;

typedef struct client {
    struct sockaddr_in user_addr;
    struct client *next;
}client;

struct client *gClientListHead = NULL;
struct sockaddr_in gServer_addr;
struct sockaddr_in gClient_addr;

int gWaitRecv = 0;

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("pls input Server IP & Server Port \n");
        return 1;
    }

    int sock_fd;
    pthread_t recvThread;

    gServer_addr.sin_family = AF_INET;
    gServer_addr.sin_addr.s_addr = inet_addr(argv[1]);
    gServer_addr.sin_port = htons(atoi(argv[2]));
    
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        printf("socket errer \n");
        return 1;
    }
    
    if ( bind(sock_fd, (struct sockaddr*) &gServer_addr, sizeof(gServer_addr)) < 0 ) {
        printf("bind errer \n");
        return 1;
    }

    printf("Server open \n");
    pthread_create(&recvThread, NULL, recvFuncThread, &sock_fd);

    while (1) {
        char inputMsg[32];
        printf("input Server Commend \n");
        printf("1. exit \n");
        fgets(inputMsg, 32, stdin);
        if (strcmp(inputMsg, "exit\n") == 0) {
            break;
        }
    }

    pthread_cancel(recvThread);

    return 0;
}

void *recvFuncThread(void *sock_fd) {
    int serv_fd = *(int *) sock_fd;
    int addr_len = sizeof(struct sockaddr_in);
    packetInfo recvInfo;
    while (1) {
        if (recvfrom(serv_fd, &recvInfo, sizeof(packetInfo), 0, (struct sockaddr*)&gClient_addr, &addr_len) < 0) {
            printf("recv thread error \n");
            pthread_exit(0);
        }

        printf("recv from user: %s", recvInfo.userName);

        switch (recvInfo.format) {
            case 0: // register
                
                break;
            case 1: // send msg to all client
                break;
            case 2: // user exit 
                break;
            default: // unknow condition
                printf("user: %s send unknow format", recvInfo.userName);
                break;
        }
    }
}
