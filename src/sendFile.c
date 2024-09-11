#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include "AES.h"
#include "KeyStorageGlobal.h"
#include <stdint.h>

#define BUFSIZE 1024

uint8_t m_aes_keys_original[16];

void sendFile(int *client_sock, char *file_path) {
  FILE *fp = fopen(file_path, "rb");
  if (fp == NULL) {
    printf("[-] Can't open file for transfer");
    close(*client_sock);
    return;
  }

  // getting file size
  long long file_size;

  // getting file name
  const char *file_name = strrchr(file_path, '/');
  if (file_name) {
    file_name++;
  } else {
    file_name = file_path;
  }

  // setting up an structure to be sent
  struct fileInfo fi;

  strcpy(fi.name, file_name);

  // printf("[.] Total content size: %lld\n", fi.size);
  // printf("[.] Content name: %s\n", fi.name);
  // printf("[.] Content type: %c\n", fi.type);

  char buffer[BUFSIZE];

  int n;

  // encrypt the original file
  encryptFile(file_path, "./encrypted.enc", m_aes_keys_original);

  fp = fopen("./encrypted.enc", "rb");

  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  fi.type = 'F';
  fi.size = file_size;

  // sending the file information to the receiver
  send(*client_sock, &fi, sizeof(fi), 0);

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
