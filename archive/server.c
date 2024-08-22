#include "../src/headerConfig.c"
#include "./getServerSocket.c"
#include <stdio.h>

int main() {
  int *clientConn = getServerSocket();

  int continueSending = 1;
  char buffer[256];
  while (continueSending) {
    memset(buffer, '\0', sizeof(buffer));
    recv(*clientConn, buffer, sizeof(buffer), 0);
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
      send(*clientConn, buffer, sizeof(buffer), 0);
      if (strncmp(buffer, "exit", 4) == 0) {
        printf("[.] Server exiting...\n");
        continueSending = 0;
      }
    }
  }

  close(*clientConn);

  free(clientConn);
  return 0;
}
