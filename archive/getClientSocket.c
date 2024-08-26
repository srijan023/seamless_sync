#include "../include/UDPListen.h"
#include "../include/UDPSend.h"
#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include <sys/socket.h>
#include <unistd.h>

/**
 * Function: getclientSocket
 * ---------------
 * Takes in the ip address of the server and create a TCP socket connecting to
 * that server and returns that socket once the connection has been established
 *
 * Parameters:
 * char* ip (ip address of the server we wish to connect)
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
 *  it has to be a valid ip address to the server
 *  the returned type is a pointer to an int.
 *
 * Example Usage:
 *  int *clientSocket = getClientSocket("127.0.0.1");
 *
 * Notes:
 *  check out the client.c file in archive for further insight
 *  check out customDataTypes
 */

struct socketInfo *getClientSocket(char *ip) {
  struct socketInfo *ans;
  memset(ans->pub_key, '\0', sizeof(ans->pub_key));

  int *clientSocket = (int *)malloc(sizeof(int));

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_port = htons(12345);

  *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    perror("[-] Socket could not be created\n");
    free(clientSocket);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  if (setsockopt(*clientSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                 sizeof(int)) < 0) {
    printf("[-] Error while setting socket reuseable");
    close(*clientSocket);
    free(clientSocket);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  printf("[+] Socket created successfully\n");

  struct customAddInfo message = listenUDP("12345", ip);
  if (message.status == 0) {
    sleep(2);
  }

  char *res = "Ready to connect";
  sendUDP(res, strlen(res), "12345", ip, 2.0);

  if (connect(*clientSocket, (struct sockaddr *)&server, sizeof(server)) != 0) {
    perror("[-] Connection failed\n");
    close(*clientSocket);
    free(clientSocket);
    ans->status = -1;
    ans->socket = NULL;
    return ans;
  }

  printf("[+] Connected to ther server\n");
  printf("\n");

  // TODO: insert the public key of peer here.
  ans->socket = clientSocket;
  ans->status = 0;

  return ans;
}
