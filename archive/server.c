#include "../src/headerConfig.c"
#include "./getServerSocket.c"
#include "./receiveFile.c"
#include "./sendFile.c"
#include <stdio.h>

int main() {
  int *connSocket = getServerSocket("192.168.88.56");

  int continueSending = 1;
  char buffer[256];
  while (continueSending) {
    memset(buffer, '\0', sizeof(buffer));
    recv(*connSocket, buffer, sizeof(buffer), 0);
    printf("Them: %s\n", buffer);
    if (strncmp(buffer, "exit", 4) == 0) {
      printf("[.] Server exiting...\n");
      continueSending = 0;
    }
    if (strncmp(buffer, "/file", 5) == 0) {
      printf("[+] Receiving files\n");
      receiveFile(connSocket);
    }
    memset(buffer, '\0', sizeof(buffer));
    if (continueSending) {
      int n = 0;
      printf("Us: ");
      while ((buffer[n++] = getchar()) != '\n')
        ;
      send(*connSocket, buffer, sizeof(buffer), 0);
      if (strncmp(buffer, "exit", 4) == 0) {
        printf("[.] Server exiting...\n");
        continueSending = 0;
      }
      if (strncmp(buffer, "/file", 5) == 0) {
        printf("Sending files...\n");
        printf("Enter the path of the file you wish to send: ");
        char fileName[20];
        scanf("%s", fileName);
        sendFile(connSocket, fileName);
        char *message = "File transfer done\n";
        send(*connSocket, message, strlen(message), 0);
      }
    }
  }

  close(*connSocket);

  free(connSocket);
  return 0;
}
