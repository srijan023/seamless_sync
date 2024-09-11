#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include "AES.h"
#include "KeyStorageGlobal.h"
#include <stdint.h>
#include <stdio.h>

#define BUFSIZE 1024

uint8_t m_aes_keys_original[16];

void receiveFile(int *client_sock, char *file_name) {
  struct fileInfo fi;
  recv(*client_sock, &fi, sizeof(fi), 0);
  sprintf(file_name, "%s", fi.name);

  printf("\n[.] Total content size: %lld\n", fi.size);
  printf("[.] Content name: %s\n", fi.name);
  printf("[.] Content type: %c\n", fi.type);

  // getting the home directory ~/
  char *home_dir = getenv("HOME");
  if (home_dir == NULL) {
    perror("[-] Could not get the home directory");
    return;
  }

  // location to save the file is ~/Documents/seamless/
  char folder_path[512];
  sprintf(folder_path, "%s/Documents/seamless", home_dir);

  // check if directory already exists
  if (access(folder_path, F_OK) == -1) {
    if (mkdir(folder_path, 0777) == -1) {
      perror("[-] Could not create the directory");
      return;
    }
  }

  char file_path[512];
  sprintf(file_path, "%s/%s", folder_path, fi.name);

  long long remaining_size = fi.size;

  char buffer[BUFSIZE];

  // opening the file to write on;
  FILE *fp = fopen("./encrypted.enc", "wb");
  if (fp == NULL) {
    perror("[-] Could not open file\n");
    return;
  }

  int n;
  while (remaining_size > 0) {
    // printf("remaining size: %lld\n", remaining_size);

    n = recv(*client_sock, buffer, BUFSIZE, 0);
    if (n > remaining_size) {
      n = remaining_size;
    }

    remaining_size -= n;

    fwrite(buffer, 1, n, fp);
  }

  printf("Printing the keys\n");
  for (int i = 0; i < 16; i++) {
    printf("%u\t", m_aes_keys_original[i]);
  }
  printf("\n");

  fclose(fp);

  printf("[+] Decryption started\n");
  decryptFile("./encrypted.enc", file_path, m_aes_keys_original);
  printf("[+] Decryption ended\n");

  return;
}
