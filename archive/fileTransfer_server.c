#include "../src/headerConfig.c"
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 2326
#define BACKLOG 5
#define BUFSIZE                                                                \
  1024 * 8 // setting up the buffer size to be 8kb for efficient transfer

int main() {
  int err;
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;

  // creating a socket
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock == -1) {
    printf("[-] Socket creation failed");
    return -1;
  }

  // setting the server_addr inside content to 0
  memset(&server_addr, 0, sizeof(server_addr));

  // setting only the necessary options
  // using ipv4
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  err = bind(server_sock, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr));
  if (err == -1) {
    printf("[-] Error in binding");
    return -1;
  }

  err = listen(server_sock, BACKLOG);
  if (err == -1) {
    printf("[-] Error while listening");
    return -1;
  }

  // server is listening on PORT

  // accepting the client request
  socklen_t sin_size = sizeof(struct sockaddr_in);
  client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &sin_size);

  if (client_sock == -1) {
    printf("[-] client accept failed");
    return -1;
  }

  printf("[+] Connected to the client %s\n", inet_ntoa(client_addr.sin_addr));

  // creating a buffer for file path
  char file_path[BUFSIZE];

  printf("Enter the file path you want to send to the client: ");
  scanf("%s", file_path);

  struct stat st;
  if (stat(file_path, &st) == -1) {
    printf("[-] Stat error\n");
    return -1;
  }

  if (S_ISDIR(st.st_mode)) {
    // it is a directory
    // sendDir()
  } else {
    // it is a file
    // sendFile()
  }

  close(client_sock);
  close(server_sock);

  return 0;
}
