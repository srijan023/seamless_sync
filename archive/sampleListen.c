#include<winsock2.h>
#include<stdio.h>
#include<ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup() failed\n");
        WSACleanup();
        return 1;
    }


    struct addrinfo hints, * res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    const int port = 12345;

    int status = getaddrinfo("127.0.0.1", "12345", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo failed: %d\n", status);
        WSACleanup();
        return 1;
    }

    // creating a udp socket
    int sockfd;
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        fprintf(stderr, "Socket creation failed");
        WSACleanup();
        return 1;
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "Could not bind socket");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(12345);
    // addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));


    // const int BACKLOG = 10; // how many pending connections in queue
    // listen(sockfd, BACKLOG);

    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    addr_size = sizeof(their_addr);

    // int new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);

    // to store the received message;
    char buffer[1024];

    // int bytes_received = recv(new_fd, buffer, sizeof(buffer), 0);
    // if (bytes_received == -1) {
    //     fprintf(stderr, "receive failed\n");
    //     closesocket(new_fd);
    //     closesocket(sockfd);
    //     freeaddrinfo(res);
    //     WSACleanup();
    //     return 1;
    // }
    int recv_length;
    if ((recv_length = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&their_addr, &addr_size)) == -1) {
        fprintf(stderr, "receiving failed\n");
        closesocket(sockfd);
        // freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    buffer[recv_length] = '\0';

    printf("Message is: %s\n", buffer);

    // closesocket(new_fd);
    closesocket(sockfd);
    // freeaddrinfo(res);
    WSACleanup();
    return 0;
}
