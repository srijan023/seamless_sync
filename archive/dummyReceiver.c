#include "../src/headerConfig.c"

int main() {
    char* group = "239.255.255.250";
    const int port = 1900;

    WSADATA wsaData;
    // Initialize Winsock2.2
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        perror("WSA startup failed!");
        return -1;
    }

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("Socket can't be created\n");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    size_t addrlen = sizeof(addr);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Binding error");
        return -1;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0) {
        perror("Set socket opt failed");
        return -1;
    }

    while (1) {
        char msgbuffer[256];
        int addrlen = sizeof(addr);
        int nbytes = recvfrom(fd, msgbuffer, 256, 0, (struct sockaddr*)&addr, &addrlen);
        if (nbytes < 0) {
            perror("Failed receiving\n");
            // return -1;
        }
        msgbuffer[nbytes] = '\0';
        printf("%s\n", msgbuffer);
    }
    WSACleanup();
    return 0;
}