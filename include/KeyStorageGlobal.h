#ifndef KEYGLOBAL_H
#define KEYGLOBAL_H
// for RSA algorithm
#include "customDataTypes.h"
#include <stdint.h>
extern long long m_rsa_e;
extern long long m_rsa_d;
extern long long m_rsa_n;
extern long long m_rsa_phi;

extern long long t_rsa_e;
extern long long t_rsa_n;

extern struct publicKeyStore *rsa_p_list;
extern int clients;

// for AES algorithm
extern uint8_t m_aes_keys_original[16];
#endif // !KEYGLOBAL_H
