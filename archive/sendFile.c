#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"

#define BUFSIZE 1024

void sendFile(int *client_sock, char *file_path) {
  FILE *fp = fopen(file_path, "rb");
  if (fp == NULL) {
    printf("[-] Can't open file for transfer");
    close(*client_sock);
    return;
  }

  // getting file size
  long long file_size;

  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // getting file name
  const char *file_name = strrchr(file_path, '/');
  if (file_name) {
    file_name++;
  } else {
    file_name = file_path;
  }

  // setting up an structure to be sent
  struct fileInfo fi;

  fi.type = 'F';
  fi.size = file_size;
  strcpy(fi.name, file_name);

  printf("remaining size: %lld\n", fi.size);
  printf("File name is %s\n", fi.name);
  printf("File type is %c\n", fi.type);

  // sending the file information to the receiver
  send(*client_sock, &fi, sizeof(fi), 0);

  char buffer[BUFSIZE];

  int n;

  // send file
  while (file_size > 0) {
    n = fread(buffer, 1, BUFSIZE, fp);
    if (n < BUFSIZE) {
      // padding the sending message such that the entire buffer is filled every
      // single time.
      memset(buffer + n, 0, BUFSIZE - n);
    }
    file_size -= n;
    send(*client_sock, buffer, n, 0);
  }

  fclose(fp);

  return;
}
