#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include <asm-generic/socket.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 2326
#define BACKLOG 5
#define BUFSIZE                                                                \
  1024 * 8 // setting up the buffer size to be 8kb for efficient transfer

/**
 * Function: findFileSize
 * ---------------
 * This function takes in a filePath and tells us the size of that file.
 *
 * Parameters:
 * char* file_path
 *
 * Returns:
 *  long long
 *
 * Side Effects:
 *  None
 *
 * Constraints:
 *  file_path must be a valid path to the file and is a string
 *
 * Example Usage:
 *  filePath("./hello.c")
 *
 * Notes:
 *  None
 */
long long findFileSize(char *file_path) {
  FILE *fp = fopen(file_path, "rb");
  if (fp == NULL) {
    printf("[-] Could not open file\n");
    return -1;
  }

  fseek(fp, 0, SEEK_END);
  long long size = ftell(fp);
  fclose(fp);
  return size;
}

void send_file(int client_sock, char *file_path, long long remaining_size) {
  FILE *fp = fopen(file_path, "rb");
  if (fp == NULL) {
    printf("[-] Can't open the file ");
    close(client_sock);
    return;
  }
  char buffer[BUFSIZE];

  int n;

  // if the size of the file is greater than the bufsize, files are sent in
  // parts
  while (remaining_size > 0) {
    n = fread(buffer, 1, BUFSIZE, fp);
    if (n < BUFSIZE) {
      // if the size of the file read is less than the bufsize, pad it with null
      // characters
      memset(buffer + n, 0, BUFSIZE - n);
    }
    remaining_size -= n;
    send(client_sock, buffer, n, 0);
  }

  fclose(fp);
  // to mark the end of file transfer, might not be needed
  send(client_sock, "E", 1, 0);
}

void send_directory(int client_sock, char *dir_path) {
  DIR *d = opendir(dir_path);
  if (d == NULL) {
    printf("[-] opendir error");
    return;
  }

  struct dirent *dir;
  while ((dir = readdir(d)) != NULL) {
    // if its . or .. skip it
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
      continue;
    }

    char path[BUFSIZE];
    // setting up the path to get information on what it is
    snprintf(path, BUFSIZE, "%s/%s", dir_path, dir->d_name);
    printf("%s\n", path);

    struct stat st;
    if (stat(path, &st) == -1) {
      printf("Error in st");
      continue;
    }

    if (S_ISDIR(st.st_mode)) {
      struct fileInfo fi;
      strcpy(fi.name, dir->d_name);
      fi.type = 'D';
      send(client_sock, &fi, sizeof(fi), 0);
      send_directory(client_sock, path);
    } else {
      struct fileInfo fi;
      strcpy(fi.name, dir->d_name);
      fi.type = 'F';
      send(client_sock, &fi, sizeof(fi), 0);
      // TODO: fix this
      send_file(client_sock, path, 1024);
    }
  }
  closedir(d);
  send(client_sock, "E", 1, 0);
}

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

  // making the socket reuseable
  if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                 sizeof(int)) < 0) {
    printf("[-] Error while setting socket reuseable");
    close(server_sock);
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
    close(server_sock);
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
    close(server_sock);
    close(client_sock);
    return -1;
  }

  const char *file_name = strrchr(file_path, '/');
  if (file_name) {
    file_name++;
  } else {
    file_name = file_path;
  }

  struct fileInfo fi;

  strcpy(fi.name, file_name);

  if (S_ISDIR(st.st_mode)) {
    // it is a directory
    fi.type = 'D';

    send(client_sock, &fi, sizeof(fi), 0);
    /*printf("Can't send directories just yet");*/
    send_directory(client_sock, file_path);
  } else {
    // it is a file
    fi.type = 'F';

    long long file_size = findFileSize(file_path);
    fi.size = file_size;

    send(client_sock, &fi, sizeof(fi), 0);

    send_file(client_sock, file_path, file_size);
  }

  close(client_sock);
  close(server_sock);

  return 0;
}
