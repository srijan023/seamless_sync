#include "../include/UDPListen.h"
#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include "AES.h"
#include "KeyStorageGlobal.h"
#include "RSA.h"
#include <netinet/tcp.h>
#include <stdint.h>

/**
 * Function: getclientSocket
 * ---------------
 * Takes in the ip address of the server and create a TCP socket connecting to
 * that server and returns that socket once the connection has been established
 *
 * Parameters:
 * char* ip (ip address of the server we wish to connect)
 *
 * Returns:
 *  returns the pointer of socket info
 *  if the status is 0 it means that the socket for the connection is received
 *  if the status is -1 it means something went wrong.
 *
 * Side Effects:
 *  NONE
 *
 * Constraints:
 *  it has to be a valid ip address to the server
 *  the returned type is a pointer to an int.
 *
 * Example Usage:
 *  int *clientSocket = getClientSocket("127.0.0.1");
 *
 * Notes:
 *  check out the client.c file in archive for further insight
 *  check out customDataTypes
 */

uint8_t m_aes_keys_original[16];
uint8_t t_aes_keys_original[16];

long long m_rsa_d;
long long m_rsa_n;

struct publicKeyStore *rsa_p_list;
int clients;

int *getClientSocket(char *ip) {

  generatingAesKey(m_aes_keys_original, sizeof(m_aes_keys_original));

  // encrypting my key
  long long encrypted_aes[16];

  long long e;
  long long n;

  int found_public_key = 0;

  // finding the public key of the client
  for (int i = 0; i <= clients; i++) {
    struct publicKeyStore pks = rsa_p_list[i];
    if (strncmp(pks.ip, ip, 20) == 0) {
      found_public_key = 1;
      e = pks.pub_e;
      n = pks.pub_n;
    }
  }

  if (!found_public_key) {
    perror("[-] Public key of client not found\n");
    exit(1);
  }

  for (int i = 0; i < 16; i++) {
    encrypted_aes[i] = rsaEncrypt(m_aes_keys_original[i], e, n);
  }

  int *clientSocket = (int *)malloc(sizeof(int));

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_port = htons(22);

  *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    perror("[-] Socket could not be created\n");
    return NULL;
  }

  if (setsockopt(*clientSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                 sizeof(int)) < 0) {
    printf("[-] Error while setting socket reusable");
    close(*clientSocket);
    free(clientSocket);
    return NULL;
  }

  printf("[+] Socket created successfully\n");

  struct customAddInfo message = listenUDP("12345", ip);
  if (message.status == 0) {
    sleep(3);
  }

  if (connect(*clientSocket, (struct sockaddr *)&server, sizeof(server)) != 0) {
    perror("[-] Connection failed\n");
    return NULL;
  }

  printf("[+] Connected to the server\n");
  printf("\n");

  printf("[+] Sending encrypted AES key\n");
  send(*clientSocket, encrypted_aes, sizeof(encrypted_aes), 0);

  printf("[+] Receiving encrypted AES key\n");
  recv(*clientSocket, encrypted_aes, sizeof(encrypted_aes), 0);

  for (int i = 0; i < 16; i++) {
    t_aes_keys_original[i] = rsaDecrypt(encrypted_aes[i], m_rsa_d, m_rsa_n);
  }

  return clientSocket;
}
