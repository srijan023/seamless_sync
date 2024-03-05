#include "../src/headerConfig.c"
#include "../include/customDataTypes.h"
#include "../include/UDPErrorHandle.h"
#include <pthread.h>

void* SSDPListen(void* arg) {
    struct customAddInfo* msg = (struct customAddInfo*)malloc(sizeof(struct customAddInfo));
#ifdef _WIN32
    WSADATA wsaData;
    // Initialize Winsock2.2
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        handleUDPError("WSAStartup failed", msg);
        return (void*)msg;
    }
#endif

    int SSDPListener = socket(AF_INET, SOCK_DGRAM, 0);
    if (SSDPListener < 0) {
        handleUDPError("Socket couldn't be created!\n", msg);
#ifdef _WIN32
        WSACleanup();
#endif
        return (void*)msg;
    }

    struct sockaddr_in theirAddrs;
    memset(&theirAddrs, 0, sizeof(theirAddrs));
    theirAddrs.sin_family = AF_INET;
    theirAddrs.sin_addr.s_addr = htonl(INADDR_ANY); // any interface
    theirAddrs.sin_port = htons(1900); // specifically for SSDP

    if (bind(SSDPListener, (struct sockaddr*)&theirAddrs, sizeof(theirAddrs)) < 0) {
        handleUDPError("Socket binding failed!\n", msg);
#ifdef _WIN32
        closesocket(SSDPListener);
        WSACleanup();
#elif __linux__
        close(SSDPListener);
#endif
        return (void*)msg;
    }

    // joining the SSDP pool
    struct ip_mreq multi_group;
    multi_group.imr_interface.s_addr = htonl(INADDR_ANY);
    multi_group.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    if (setsockopt(SSDPListener, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multi_group, sizeof(multi_group)) < 0) {
        handleUDPError("Can't join the multicast group!\n", msg);
#ifdef _WIN32
        closesocket(SSDPListener);
        WSACleanup();
#elif __linux__
        close(SSDPListener);
#endif
        return (void*)msg;
    }

    char msgBuffer[256];
    int addrlen = sizeof(theirAddrs);
    int count = 0;
    int* condition = (int*)arg;
    while (*condition) {
        int nbytes = recvfrom(SSDPListener, msgBuffer, 256, 0, (struct sockaddr*)&theirAddrs, &addrlen);
        if (nbytes > 0) {
            count++;
            msgBuffer[nbytes] = '\0';
            printf("%s\n", msgBuffer);
        }
    }
    sprintf(msg->message, "%d messages were receieved\n", count);
    msg->status = 0;
#ifdef _WIN32
    closesocket(SSDPListener);
    WSACleanup();
#elif __linux__
    close(SSDPListener);
#endif
    return msg;
}