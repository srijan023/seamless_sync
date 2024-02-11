#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    // Initialize Winsock2.2
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    char hostname[120];
    gethostname(hostname, sizeof hostname);
    printf("My hostname is: %s\n", hostname);

    // Using gethostbyname()
    /**
     * The internal structure of the hostent is:
     * char *h_name => the real canocial name
     * char **h_aliases => A list of aliases that can be accessed with arrays
     * int h_adddrtype => the result's address type
     * int length => the length of the address in bytes
     * char *h_addr_list => A list of IP address of the host.
    */
    // struct hostent* he = gethostbyname(hostname);
    // struct in_addr** addr_list = (struct in_addr**)he->h_addr_list;
    // printf("\n");
    // for (int i = 0; addr_list[i] != NULL; i++) {
    //     printf("%s \n", inet_ntoa(*addr_list[i]));
    // }
    // printf("\n");

    // using getaddrinfo() function
    struct addrinfo hints;
    struct addrinfo* res;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, "http", &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
        return 1;
    }

    struct addrinfo* itr;
    for (itr = res; itr != NULL; itr = itr->ai_next) {
        // to get address in generic form
        // as IPv4 and IPv6 have different types
        void* addr;
        char* ipver;

        if (itr->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)itr->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)itr->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        char ipstr[INET6_ADDRSTRLEN];
        // Converting IP address to string format and printing it
        inet_ntop(itr->ai_family, addr, ipstr, sizeof(ipstr));
        printf(" %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res);
    freeaddrinfo(itr);
    // NOTE: We can declare the variable itr during the iteration of the loop and don't have to explicitly free it.

    WSACleanup();

    // SSDP Protocol -> reserved address and port 1900 -> broadcast
    return 0;
}