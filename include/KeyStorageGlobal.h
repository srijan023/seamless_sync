#ifndef KEYGLOBAL_H
#define KEYGLOBAL_H
// for RSA algorithm
#include <stdint.h>
extern long long m_rsa_e;
extern long long m_rsa_d;
extern long long m_rsa_n;
extern long long m_rsa_phi;

extern long long t_rsa_e;
extern long long t_rsa_n;

// for AES algorithm
extern uint8_t aes_keys_original[16];
#endif // !KEYGLOBAL_H
