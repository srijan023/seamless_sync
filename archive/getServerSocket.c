#include "../include/UDPListen.h"
#include "../include/UDPSend.h"
#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include <netinet/in.h>
#include <sys/socket.h>

/**
 * Function: getServerSocket
 * ---------------
 * Takes in the ip address of the client we wish to connect, generates the
 * connection socket and give it back to you.
 * Under the hood it creates two socket one for the server and another for the
 * connection, the socket of server is used to listen to the incoming conneciton
 * connection requests are received across multiple interfaces
 * finally when the connection is setup, new connection socket is created which
 * is then returned.
 *
 * Parameters:
 * char* ip(takes in the ip address of the client we wish to connect to)
 *
 * Returns:
 *  returns the pointer of socket info
 *  if the status is 0 it means that the socket for the connection is received
 *  if the status is -1 it means something went wrong.
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
 *  check out customDataTypes
 */
struct socketInfo *getServerSocket(char *ip) {

  struct socketInfo *ans;
  memset(ans->pub_key, '\0', sizeof(ans->pub_key));

  struct sockaddr_in servAddr, clientAddr;
  int servSocket;

  int *clientConn = (int *)malloc(sizeof(int));

  servSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (servSocket == -1) {
    perror("[-] Socket creation failed\n");
    free(clientConn);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  if (setsockopt(servSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0) {
    printf("[-] Error while setting socket reuseable");
    close(servSocket);
    free(clientConn);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  printf("[+] socket create successfully");
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY; // accept connection from any interface
  servAddr.sin_port = htons(12345);

  int status = bind(servSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));

  if (status != 0) {
    perror("[-] Binding to the socket failed");
    free(clientConn);
    close(servSocket);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }
  printf("[+] Socekt successfully bound to server address\n");

  status = listen(servSocket, 5);

  if (status != 0) {
    perror("[-] Listening for incoming connection failed\n");
    free(clientConn);
    close(servSocket);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  char *message = "Listening for the connection";
  sendUDP(message, strlen(message), "12345", ip, 3.0);

  struct customAddInfo res = listenUDP("12345", ip);
  if (res.status != 0) {
    free(clientConn);
    close(servSocket);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  printf("[+] Listening for incoming connections\n");

  int len = sizeof(clientAddr);
  *clientConn =
      accept(servSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&len);

  if (clientConn < 0) {
    perror("[-] Client Accept failed\n");
    free(clientConn);
    close(servSocket);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  printf("[+] Client accepted\n");

  ans->socket = clientConn;
  ans->status = 0;

  return ans;
}
