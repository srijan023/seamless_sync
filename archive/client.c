#include "./getClientSocket.c"
#include "./receiveFile.c"
#include "./sendFile.c"

int main() {
  int *connSocket = getClientSocket("127.0.01");

  int continueCommunication = 1;
  char buffer[256];
  while (continueCommunication) {
    memset(buffer, '\0', sizeof(buffer));
    int n = 0;
    printf("Us: ");
    while ((buffer[n++] = getchar()) != '\n')
      ;
    send(*connSocket, buffer, sizeof(buffer), 0);
    if (strncmp(buffer, "exit", 4) == 0) {
      continueCommunication = 0;
    }
    if (strncmp(buffer, "/file", 5) == 0) {
      printf("[+] Sending files \n");
      printf("Enter the path of the file you wish to send: ");
      char fileName[20];
      scanf("%s", fileName);
      sendFile(connSocket, fileName);
      char *message = "File transfer done";
      send(*connSocket, message, strlen(message), 0);
    }
    memset(buffer, '\0', sizeof(buffer));
    if (continueCommunication) {
      recv(*connSocket, buffer, sizeof(buffer), 0);
      printf("Them: %s\n", buffer);
      if (strncmp(buffer, "exit", 4) == 0) {
        continueCommunication = 0;
      }
      if (strncmp(buffer, "/file", 5) == 0) {
        printf("[+] Receiving files\n");
        receiveFile(connSocket);
      }
    }
  }
  close(*connSocket);
  free(connSocket);
  return 0;
}
