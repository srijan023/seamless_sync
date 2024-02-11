#include<winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        printf("WSAStartup() failed");
        return 1;
    }

    // creating a udp socket
    int sockfd; // file descripter for socket
    // socket(domain either ipv4 or ipv6, type tcp or udp, protocol 0 (self choose) or IPPROTO_UDP/TCP)
    // selected for future proofing or better documentation
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) {
        fprintf(stderr, "Can't create a socket");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in dest_addr;
    /*
        struct sockaddr_in {
            short int sin_family; // Address family, AF_INET (always)
            unsigned short int sin_port; // Port number
            struct in_addr sin_addr; // Internet address
            unsigned char sin_zero[8]; // Padding to make the structure the same size as struct sockaddr
        };
    */
    // socklen_t addr_len;
    char* message = "Hello this is loopback";
    const int port = 12345;

    // set every byte of dest_addr to 0  
    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // sendto() used with UDP where previous connections are not made 
    // sockfd -> file descriptor of the socket through which data will be sent
    // message -> data to be sent (can be a pointer to the memory location where data is present)
    // length of the message
    // optional flags
    // destination address -> here sockaddr_in is converted to sockaddr
    // length of destination address

    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == -1) {
        fprintf(stderr, "Failure in sendto");
        closesocket(sockfd);
        // shutdown(sockfd, 2);
        return 1;
    }
    printf("Message sent successfully\n");
    closesocket(sockfd);
    // shutdown(sockfd, 2);
    WSACleanup();
    return 0;
}