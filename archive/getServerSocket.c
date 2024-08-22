#include "../src/headerConfig.c"

int *getServerSocket() {
  struct sockaddr_in servAddr, clientAddr;
  int servSocket;

  int *clientConn = (int *)malloc(sizeof(int));

  servSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (servSocket == -1) {
    perror("[-] Socket creation failed\n");
    return NULL;
  }

  printf("[+] socket create successfully");
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(
      "127.0.0.1"); // get this as a function parameter and replace this
  servAddr.sin_port = htons(12345);

  int status = bind(servSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));

  if (status != 0) {
    perror("[-] Binding to the socket failed");
    return NULL;
  }
  printf("[+] Socekt successfully bound to server address\n");

  status = listen(servSocket, 5);

  if (status != 0) {
    perror("[-] Listening for incoming connection failed\n");
    return NULL;
  }

  printf("[+] Listening for incoming connections\n");

  int len = sizeof(clientAddr);
  *clientConn =
      accept(servSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&len);

  if (clientConn < 0) {
    perror("[-] Client Accept failed\n");
    return NULL;
  }

  printf("[+] Client accepted\n");

  return clientConn;
}
