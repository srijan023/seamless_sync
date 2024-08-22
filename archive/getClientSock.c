#include "../src/headerConfig.c"

int *getClientSock() {
  int *clientSocket = (int *)malloc(sizeof(int));

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(12345);

  *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    perror("[-] Socket could not be created\n");
    return NULL;
  }

  printf("[+] Socket created successfully\n");

  if (connect(*clientSocket, (struct sockaddr *)&server, sizeof(server)) != 0) {
    perror("[-] Connection failed\n");
    return NULL;
  }

  printf("[+] Connected to ther server\n");
  printf("\n");

  return clientSocket;
}
