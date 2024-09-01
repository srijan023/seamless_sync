#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024 * 8

void receiveFile(int *client_sock) {
  char file_name[100];
  struct fileInfo fi;
  recv(*client_sock, &fi, sizeof(fi), 0);
  sprintf(file_name, "temp_%s", fi.name);

  long long remaining_size = fi.size;
  printf("remaining size: %lld\n", remaining_size);

  char buffer[BUFSIZE];

  // opening the file to write on;
  FILE *fp = fopen(file_name, "wb");
  if (fp == NULL) {
    perror("[-] Could not open file\n");
    return;
  }

  int n;
  while (remaining_size > 0) {
    printf("remaining size: %lld\n", remaining_size);

    n = recv(*client_sock, buffer, BUFSIZE, 0);
    if (n > remaining_size) {
      n = remaining_size;
    }

    remaining_size -= n;

    fwrite(buffer, 1, n, fp);
  }

  fclose(fp);

  return;
}
