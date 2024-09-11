#include "../include/UDPSend.h"
#include "AES.h"
#include "KeyStorageGlobal.h"
#include "RSA.h"
#include "customDataTypes.h"
#include "headerConfig.c"
#include <netinet/tcp.h>
#include <stdint.h>
#include <unistd.h>

/**
 * Function: getServerSocket
 * ---------------
 * Takes in the ip address of the client we wish to connect, generates the
 * connection socket and give it back to you.
 * Under the hood it creates two socket one for the server and another for the
 * connection, the socket of server is used to listen to the incoming connection
 * connection requests are received across multiple interfaces
 * finally when the connection is setup, new connection socket is created which
 * is then returned.
 *
 * Parameters:
 * char* ip(takes in the ip address of the client we wish to connect to)
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
 *  has to be a valid ip address
 *  return type is a pointer to an int
 *
 * Example Usage:
 *  int *serverSocket = getServerSocket("127.0.0.1");
 *
 * Notes:
 *  check out server.c file in archive for further insight.
 *  check out customDataTypes
 */
uint8_t m_aes_keys_original[16];
uint8_t t_aes_keys_original[16];

long long m_rsa_d;
long long m_rsa_n;

struct publicKeyStore *rsa_p_list;
int clients;

int *getServerSocket(char *ip) {
  struct sockaddr_in servAddr, clientAddr;
  int servSocket;

  int *clientConn = (int *)malloc(sizeof(int));

  servSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (servSocket == -1) {
    perror("[-] Socket creation failed\n");
    return NULL;
  }

  if (setsockopt(servSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0) {
    printf("[-] Error while setting socket reusable");
    close(servSocket);
    return NULL;
  }

  printf("[+] socket create successfully\n");
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY; // accept connection from any interface
  servAddr.sin_port = htons(12345);

  int status = bind(servSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));

  if (status != 0) {
    perror("[-] Binding to the socket failed");
    return NULL;
  }
  printf("[+] Socket successfully bound to server address\n");

  status = listen(servSocket, 5);

  if (status != 0) {
    perror("[-] Listening for incoming connection failed\n");
    return NULL;
  }

  char *message = "Listening for the connection";
  sendUDP(message, strlen(message), "12345", ip, 3.0);

  printf("[+] Listening for incoming connections\n");

  int len = sizeof(clientAddr);
  *clientConn =
      accept(servSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&len);

  if (clientConn < 0) {
    perror("[-] Client Accept failed\n");
    return NULL;
  }

  printf("[+] Client accepted\n");

  printf("[+] Receiving encrypted AES key\n");
  uint8_t encrypted_aes[16];

  recv(*clientConn, encrypted_aes, sizeof(encrypted_aes), 0);
  for (int i = 0; i < 16; i++) {
    t_aes_keys_original[i] = rsaDecrypt(encrypted_aes[i], m_rsa_d, m_rsa_n);
  }

  printf("Their aes keys are: \n");
  for (int i = 0; i < 16; i++) {
    printf("%u\t", t_aes_keys_original[i]);
  }
  printf("\n");

  generatingAesKey(m_aes_keys_original, sizeof(m_aes_keys_original));

  printf("Printing my aes keys: \n");
  for (int i = 0; i < 16; i++) {
    printf("%u\t", m_aes_keys_original[i]);
  }

  printf("\n");

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

  printf("Their public keys are %lld %lld\n", e, n);

  for (int i = 0; i < 16; i++) {
    encrypted_aes[i] = rsaEncrypt(m_aes_keys_original[i], e, n);
  }

  printf("[+] Sending encrypted AES key");
  send(*clientConn, encrypted_aes, sizeof(encrypted_aes), 0);
  printf("I am sending\n");
  for (int i = 0; i < 16; i++) {
    printf("%u\t", encrypted_aes[i]);
  }
  printf("\n");

  return clientConn;
}
