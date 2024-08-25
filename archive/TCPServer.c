#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"

int main() {
#ifdef _WIN32
  struct customAddInfo msg;
  WSADATA wsaData;
  // Initialize Winsock2.2
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    perror("WSAStartup");
    return 1;
  }
#endif
  struct sockaddr_in serverAddr, clientAddr;
  int servSocket, clientConn;

  servSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (servSocket == -1) {
    perror("Socket creation failed of server\n");
    return 1;
  }
  printf("[+] Socket successfully created\n");
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr =
      inet_addr("127.0.0.1"); // checking for loopback to work, replace it with
                              // htonl(INADDR_ANY)
  serverAddr.sin_port = htons(12345);

  // binding the socket to the server address
  int status =
      bind(servSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  if (status != 0) {
    perror("[-] Binding of socket to server address failed!\n");
    return 1;
  }
  printf("[+] Socket successfully bound to server address!\n");

  // Listening for incoming connections
  status = listen(servSocket, 5); // socket and backlog
  if (status != 0) {
    perror("[-] Listening for incoming connections failed!\n");
    return 1;
  }
  printf("[+] Listening for incoming connections\n");

  // Accept the incoming client

  int len = sizeof(clientAddr);
  clientConn =
      accept(servSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&len);
  if (clientConn < 0) {
    perror("[-] Accept failed!\n");
    return 1;
  }
  printf("[+] Client accepted\n");
  printf("\n");

  // Handle communication
  int continueSending = 1;
  char buffer[256];
  while (continueSending) {
    memset(buffer, '\0', sizeof(buffer));
    recv(clientConn, buffer, sizeof(buffer), 0);
    printf("Them: %s\n", buffer);
    if (strncmp(buffer, "exit", 4) == 0) {
      printf("[.] Server exiting...\n");
      continueSending = 0;
    }
    memset(buffer, '\0', sizeof(buffer));
    if (continueSending) {
      int n = 0;
      printf("Us: ");
      while ((buffer[n++] = getchar()) != '\n')
        ;
      send(clientConn, buffer, sizeof(buffer), 0);
      if (strncmp(buffer, "exit", 4) == 0) {
        printf("[.] Server exiting...\n");
        continueSending = 0;
      }
    }
  }

  close(clientConn);
  close(servSocket);
#ifdef _win32
  WSACleanup();
#endif
  return 0;
}
