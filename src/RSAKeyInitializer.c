#include "KeyStorageGlobal.h"
#include "RSA.h"
#define BITLENGTH 30

// initialize the public and private keys for RSA algorithm during the initial
// phase of program execution for AES it would be done during the TCP
// connection.

long long m_rsa_e;
long long m_rsa_d;
long long m_rsa_n;
long long m_rsa_phi;

// executed before the main function to initialize the rsa keys
__attribute__((constructor)) void initializeRSAKeys() {
  // Initialize random seed
  srand(time(NULL));
  printf("[+] Generating encryption keys\n");
  m_rsa_phi = generateRSAKeys(BITLENGTH, &m_rsa_e, &m_rsa_d, &m_rsa_n);
}
