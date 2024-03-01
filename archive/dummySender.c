#include "../src/headerConfig.c"
#include "../src/UDPSend.c"

typedef struct customAddInfo SSDPSendMessage;
int main() {
    char* group = "239.255.255.250";
    char* port = "1900";

    char* message = "hello world";
    // format: message, message length, port: 1900 for SSDP, 
    //ip: "239.255.255.250" for SSDP and sendMultiple? 0 -> false 1-> true
    SSDPSendMessage msg = sendUDP(message, strlen(message), port, group, 1);
    printf("%s\n", msg.message);

    return 0;
}

// Previous code
    // WSADATA wsaData;
    // // Initialize Winsock2.2
    // int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    // if (iResult != 0) {
    //     perror("WDS Startup");
    //     return -1;
    // }

    // int fd = socket(AF_INET, SOCK_DGRAM, 0);
    // if (fd < 0) {
    //     perror("Socket can' be created");
    //     return -1;
    // }

    // struct sockaddr_in addr;
    // memset(&addr, 0, sizeof(addr));
    // addr.sin_family = AF_INET; // IPv4
    // addr.sin_addr.s_addr = inet_addr(group); // put my ip address automatically
    // addr.sin_port = htons(port);

    // while (1) {
    //     char ch = 0;
    //     int nbytes = sendto(fd, message, strlen(message), 0, (struct sockaddr*)&addr, sizeof(addr));
    //     if (nbytes < 0) {
    //         perror("Message could not be sent");
    //         // return -1;
    //     }
    //     Sleep(1000);
    // }
    // WSACleanup();
