#include "../src/headerConfig.c"

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        perror("WSAStartup failed\n");
        return 1;
    }
    int clientSocket;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // server ip address
    server.sin_port = htons(12345);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("[-] socket could not be created\n");
        return 1;
    }
    printf("[+] Socket creted successfully\n");

    if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) != 0) {
        perror("[-] connect failed\n");
        return 1;
    }
    printf("[+] Connected to server\n");
    printf("\n");

    int continueCommunication = 1;
    char buffer[256];
    while (continueCommunication) {
        memset(buffer, '\0', sizeof(buffer));
        int n = 0;
        printf("Us: ");
        while ((buffer[n++] = getchar()) != '\n')
            ;
        send(clientSocket, buffer, sizeof(buffer), 0);
        if (strncmp(buffer, "exit", 4) == 0) {
            continueCommunication = 0;
        }
        memset(buffer, '\0', sizeof(buffer));
        if (continueCommunication) {
            recv(clientSocket, buffer, sizeof(buffer), 0);
            printf("Them: %s\n", buffer);
            if (strncmp(buffer, "exit", 4) == 0) {
                continueCommunication = 0;
            }
        }
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}