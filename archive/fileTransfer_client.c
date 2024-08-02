#include "../src/headerConfig.c"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 2326
#define BACKLOG 5
#define BUFSIZE 1025 * 8

int main() {
  int server_sock;

  struct sockaddr_in server_addr;
  char *server_address = "127.0.0.1";

  // creating a socket
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock == -1) {
    printf("[-] Socket creation failed");
    return -1;
  }

  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(server_sock, (struct sockaddr *)&server_addr,
              sizeof(struct sockaddr)) == -1) {
    printf("[-] Connection to client failed");
    return -1;
  }

  // here we can send any messages to server.

  // connected to server
  char type;
  char name[BUFSIZE];

  // these two can be combined to one
  recv(server_sock, &type, 1, 0);
  recv(server_sock, name, BUFSIZE, 0);

  if (type == 'D') {
    // it is a directory
    // recvDir()
  } else {
    // it is a file
    // recvFile()
  }

  close(server_sock);
  return 0;
}
