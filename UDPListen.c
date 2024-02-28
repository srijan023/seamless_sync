#include "headerConfig.c"
#include "customDataTypes.h"

typedef struct customAddInfo customMsg;
void handleErrorListen(char* errMsg, customMsg* msg) {
    msg->status = -1;
    strcpy(msg->message, errMsg);
    return;
}

customMsg listenUDP(char* port, char* addr) {
    customMsg returnMessage;

#ifdef _WIN32
    WSADATA wsaData;
    // Initialize Winsock2.2
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        handleErrorListen("WSAStartup failed", &returnMessage);
        return returnMessage;
    }
#endif
    struct addrinfo hints, * res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(addr, port, &hints, &res);
    if (status != 0) {
        handleErrorListen("Can't get the address info", &returnMessage);
#ifdef _WIN32
        WSACleanup();
#endif
        return returnMessage;
    }

    // creating a UDP socket
    int sockfd;
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        handleErrorListen("Can't create a socket!", &returnMessage);
#ifdef _WIN32
        WSACleanup();
#endif
        return returnMessage;
    }

    status = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (status == -1) {
        handleErrorListen("Can't bind socket to the port!", &returnMessage);
#ifdef _WIN32
        WSACleanup();
#endif
        return returnMessage;
    }

    struct sockaddr_in their_addr;
    socklen_t addr_size;

    addr_size = sizeof(their_addr);

    char buffer[1024];

    // status here gets the value -1 if receiving was failed, else its gets the size of bytes received
    status = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&their_addr, &addr_size);
    if (status == -1) {
        handleErrorListen("Message receiving failed!", &returnMessage);
#ifdef _WIN32
        WSACleanup();
#endif
        return returnMessage;
    }

    buffer[status] = '\0';
    returnMessage.status = 0;
    strcpy(returnMessage.message, buffer);
#ifdef _WIN32
    WSACleanup();
#endif
    return returnMessage;
}