#include "../src/headerConfig.c"
#include "../src/UDPSend.c"
#include <pthread.h>

typedef struct SSDPThreadMessage {
    int sockfd;
    struct sockaddr_in addr;
}ThreadMessage;

void* listenToPackets(void* message) {
    ThreadMessage* msg = message;
    char buffer[1024];
    socklen_t add_size = sizeof(msg->addr);
    int status;
    while (1) {
        status = recvfrom(msg->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&(msg->addr), &add_size);
        if (status == -1) {
            perror("receiving failed!");
            WSACleanup();
        }

        buffer[status] = '\0';
        printf("%s\n", buffer);
    }
}

void* sendingPackets() {
    while (1) {
        char* message = "This is a SSDP message";
        // Sleep(3000);
        sendUDP(message, strlen(message), "239.255.255.250", "1900");
    }
}
int main() {
#ifdef _WIN32
    WSADATA wsaData;
    // Initialize Winsock2.2
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        // handleErrorListen("WSAStartup failed", &returnMessage);
        return -1;
        // return returnMessage;
    }
#endif

    int sockfd;
    struct ip_mreq mreq;
    struct sockaddr_in local_addr;

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a local address (optional)
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // local_addr.sin_port = htons(12345); // Your desired port
    bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr));

    // Join the multicast group
    mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250"); // Multicast group address
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); // Use any interface
    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

    // Now the socket is ready to receive multicast packets
    int status = 0;

    ThreadMessage message;
    message.sockfd = sockfd;
    message.addr = local_addr;

    pthread_t receiver_thread, sender_thread;
    int receiveStatus, sendStatus;

    receiveStatus = pthread_create(&receiver_thread, NULL, listenToPackets, (void*)&message);
    sendStatus = pthread_create(&sender_thread, NULL, sendingPackets, NULL);

    // joining the threads;
    pthread_join(receiver_thread, NULL);
    pthread_join(sender_thread, NULL);

    // status = listenToPackets(&sockfd, &local_addr);
    // status = sendingPackets();
    // Close the socket when done
    closesocket(sockfd);
    WSACleanup();

    return status;
}