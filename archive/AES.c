#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
using unsigned char for the following reasons:
- later bitwise operation would not mess up the sign
- we are storing bytes no the actual character so it is easier to interpret
*/
void generatingAesKey(uint8_t *key, size_t length) {
  // seeding the random number
  srand((unsigned int)time(NULL));

  // generating random bytes of key
  for (size_t i = 0; i < length; i++) {
    key[i] = (uint8_t)(rand() % 256);
  }
}

// Round constants for AES key expansion
// GF(2^8)
static const uint32_t Rcon[10] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
    0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000};

// TODO: Define how it is obtained, it is constant value so written
static const uint8_t Sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
    0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26,
    0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
    0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed,
    0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
    0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec,
    0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
    0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
    0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
    0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
    0xb0, 0x54, 0xbb, 0x16};

/*
a0 a1 a2 a3 -> a1 a2 a3 a0
w << 8 -> a1 a2 a3 00
w >> 24 -> 00 00 00 a1
*/
uint32_t rotateWords(uint32_t w) { return (w << 8) | (w >> 24); }

/*
word = a0 a1 a2 a3 -> result = Sbox[a0] Sbox[a1] Sbox[a2] Sbox[a3]
result -> 00 00 00 00
Sbox[word >> 24 && 0xFF] = Sbox[a0] << 24 = Sbox[a0] 00 00 00
Sbox[word >> 16 && 0xFF] = Sbox[00 00 00 a1] << 16 = 00 Sbox[a1] 00 00
therefore, result = Sbox[a0] Sbox[a1]
*/

uint32_t sBoxSubstitution(uint32_t word) {
  uint32_t result = 0;
  result |= Sbox[(word >> 24) & 0xFF] << 24;
  result |= Sbox[(word >> 16) & 0xFF] << 16;
  result |= Sbox[(word >> 8) & 0xFF] << 8;
  result |= Sbox[word & 0xFF];
  return result;
}

void keyExpansion(const uint8_t *key, uint32_t *expandedKeys) {
  uint32_t temp;
  int i = 0;

  // copying the original key as the first part of the expanded key
  while (i < 4) {
    // uint32_t stores the first4 bytes in a variable from the array of uint8_t
    /*
    if original key is:
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c

    we get:
    0x2b7e1516
    0x28aed2a6
    0xabf71588
    0x09cf4f3c
    */
    expandedKeys[i] =
        ((uint32_t)key[4 * i] << 24) | ((uint32_t)key[4 * i + 1] << 16) |
        ((uint32_t)key[4 * i + 2] << 8) | ((uint32_t)key[4 * i + 3]);
    i++;
  }

  // generating rest of the expanded keys
  while (i < 44) {
    temp = expandedKeys[i - 1];
    if (i % 4 == 0) {
      /*
      if the word to be genereted is the first word for the new set of keys then
      the previous word (the last word of the previous set of keys) has to go
      through a g block transformation which includes
      - Rotate word
      - S-box substituition
      - Addition or rotation constant
      */
      temp = sBoxSubstitution(rotateWords(temp)) ^ Rcon[(i / 4) - 1];
      // Rcon[roundNumber]
    }
    // every other words are just simply xored with the previous word
    expandedKeys[i] = expandedKeys[i - 4] ^ temp;
    i++;
  }
}

int main() {
  uint8_t key[16];
  generatingAesKey(key, sizeof(key));
  uint32_t expandedKeys[44];

  keyExpansion(key, expandedKeys);

  printf("Expanded keys: \n");
  for (int i = 0; i < 44; i++) {
    printf("%08x ", expandedKeys[i]);
    if ((i + 1) % 4 == 0)
      printf("\n");
  }

  return 0;
}
