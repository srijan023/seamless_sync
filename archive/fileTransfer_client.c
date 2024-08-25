#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 2326
#define BACKLOG 5
#define BUFSIZE 1024 * 8

void receive_file(int server_sock, char *filePath, char *name,
                  long long remaining_size) {
  char file_path[BUFSIZE];
  snprintf(file_path, BUFSIZE, "%s/%s", filePath, name);
  FILE *fp = fopen(file_path, "wb");
  if (fp == NULL) {
    printf("[-] File open error");
    return;
  }
  char buffer[BUFSIZE];
  int n;

  while (remaining_size > 0) {
    n = recv(server_sock, buffer, BUFSIZE, 0);

    if (n > remaining_size) {
      n = remaining_size;
    }

    remaining_size -= n;

    printf("Received: %d", n);
    fwrite(buffer, 1, n, fp);
  }

  recv(server_sock, buffer, BUFSIZE, 0);
  printf("This marks the end of the file receiving process \n %s", buffer);

  fclose(fp);
}

void receive_dir(int server_sock, char *dirPath, char *name) {
  // filePath -> ./
  // name = name of the directory
  char dir_path[BUFSIZE];
  snprintf(dir_path, BUFSIZE, "%s/%s", dirPath, name);

  // get the name part out of park we using "Temp Dir" for now
  mkdir(dir_path, 0755);

  struct fileInfo fi;
  while (recv(server_sock, &fi, sizeof(fi), 0) > 0) {

    if (fi.type == 'D') {
      // TODO: GET HERE FAST
      receive_dir(server_sock, dir_path, fi.name);
    } else if (fi.type == 'F') {
      // TODO: fix this
      receive_file(server_sock, dir_path, fi.name, 1024);
    }
  }
}

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
  /*recv(server_sock, &type, 1, 0);*/
  /*recv(server_sock, name, BUFSIZE, 0);*/
  struct fileInfo fi;
  recv(server_sock, &fi, sizeof(fi), 0);
  printf("The file type is %c\n", fi.type);
  printf("The name of the file is %s\n", fi.name);
  printf("The size of the file id %lld\n", fi.size);

  long long file_size = fi.size;

  if (fi.type == 'D') {
    // it is a directory
    // recvDir()
    printf("Currently can't receive a directory");
    receive_dir(server_sock, "./", "tempDir");
  } else {
    // it is a file
    // recvFile()
    receive_file(server_sock, "./", "Tempfile.c", file_size);
    /*printf("Receiving a file");*/
  }

  close(server_sock);
  return 0;
}
