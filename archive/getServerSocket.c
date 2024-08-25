#include "../include/UDPSend.h"
#include "../src/headerConfig.c"
#include <netinet/in.h>
#include <sys/socket.h>

/**
 * Function: getServerSocket
 * ---------------
 * Takes in the ip address of the client we wish to connect, generates to
 *
 *
 * Parameters:
 * char* ip(takes in the ip address of the client we wish to connect to)
 *
 * Returns:
 *  returns the pointer to the TCP socket used for communication
 *
 * Side Effects:
 *  NONE
 *
 * Constraints:
 *  has to be a valid ip address
 *  return type is a pointer to an int
 *
 * Example Usage:
 *  int *serverSocket = getServerSocket("127.0.0.1");
 *
 * Notes:
 *  check out server.c file in archive for further insight.
 */
int *getServerSocket(char *ip) {
  struct sockaddr_in servAddr, clientAddr;
  int servSocket;

  int *clientConn = (int *)malloc(sizeof(int));

  servSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (servSocket == -1) {
    perror("[-] Socket creation failed\n");
    return NULL;
  }

  if (setsockopt(servSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0) {
    printf("[-] Error while setting socket reuseable");
    close(servSocket);
    return NULL;
  }

  printf("[+] socket create successfully");
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY; // accept connection from any interface
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

  char *message = "Listening for the connection";
  sendUDP(message, strlen(message), "12345", ip, 3.0);

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
