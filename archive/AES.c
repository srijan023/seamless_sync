#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define AES_BLOCK_SIZE 16
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

// Inverse s-box
static const uint8_t InvSbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
    0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32,
    0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
    0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50,
    0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
    0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41,
    0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
    0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b,
    0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
    0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d,
    0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
    0x55, 0x21, 0x0c, 0x7d};

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

/*
[state]
s0  | s1  | s2  | s3
s4  | s5  | s6  | s7
s8  | s9  | s10 | s11
s12 | s13 | s14 | s15

[round key]
0x12345678
0x9abcdef0
0x11121314
0x15161718

[state]
s0^12  | s1^34  | s2^56  | s3^78
s4^9a  | s5^bc  | s6^de  | s7^f0
s8^11  | s9^12  | s10^13 | s11^14
s12^15 | s13^16 | s14^17 | s15^18
*/

void addRoundKey(uint8_t *state, const uint32_t *key) {
  for (int i = 0; i < 4; i++) {
    state[4 * i] ^= (key[i] >> 24) & 0xff;
    state[4 * i + 1] ^= (key[i] >> 16) & 0xff;
    state[4 * i + 2] ^= (key[i] >> 8) & 0xff;
    state[4 * i + 3] ^= (key[i]) & 0xff;
  }
}

// first step of main round
// here we substitute each value of the state matrix(4x4) with the value from
// the substituiton (encryption) or inverse substitute (decryption) box
void substituteBytes(uint8_t *state) {
  for (int i = 0; i < 16; i++) {
    state[i] = Sbox[state[i]];
  }
}

void invSubstituteBytes(uint8_t *state) {
  for (int i = 0; i < 16; i++) {
    state[i] = InvSbox[state[i]];
  }
}

/*
shift the state's rows by
0 on the left
1 on the left
2 on the left
3 on the left
*/

void shiftRow(uint8_t *state, int rowNum) {
  int i, j;
  uint8_t tmp;
  // each iteration shifts the row to the left by 1
  for (i = 0; i < rowNum; i++) {
    tmp = state[0];
    for (j = 0; j < 3; j++) {
      state[j] = state[j + 1];
    }
    state[3] = tmp;
  }
}

void invShiftRow(uint8_t *state, int rowNum) {
  int i, j;
  uint8_t tmp;
  // each iteration shifts the row to the right by 1
  for (i = 0; i < rowNum; i++) {
    tmp = state[3];
    for (j = 3; j > 0; j--) {
      state[j] = state[j - 1];
    }
    state[0] = tmp;
  }
}

void shiftRows(uint8_t *state) {
  int i;
  // iterate over the 4 rows and call shiftRow() with that row
  for (i = 0; i < 4; i++) {
    shiftRow(state + 4 * i, i);
  }
}

void invShiftRows(uint8_t *state) {
  int i;
  // iterate over the 4 rows and call invShiftRow() with that row
  for (i = 0; i < 4; i++) {
    invShiftRow(state + 4 * i, i);
  }
}

// Galois Multiplication by 2,3,9,11,13 and 14
/*
In galios multiplication by 2 first we check if the msb is 1
Then we shift the bits by one position to the left which is multiplication by 2
operation If the msb was 1, now the result has overflown.

Therefore to balance that we do modulos operation with the irreducable
polynomial which in this case of GF(2^8) is x^8 + x^4 + x^3 + x + 1 -> 0x11b

NOTE: While performing the reduction operation we use 0x1b because the 1 is the
overflow bit which is not included.
*/
uint8_t gmul_2(uint8_t val) {
  uint8_t hi_bit;
  hi_bit = (val & 0x80);
  val <<= 1;
  if (hi_bit) {
    val ^= 0x1b;
  }
  return val;
}

/*
Galois multiplication by 3 is simply galois multiplication by 2 followed by the
xor with the same value which is same as a * 3 = a * 2 + a
*/
uint8_t gmul_3(uint8_t val) { return gmul_2(val) ^ val; }

/*
Galios multiplication by 9 is 2^3 + 1
*/
uint8_t gmul_9(uint8_t val) { return gmul_2(gmul_2(gmul_2(val))) ^ val; }
uint8_t gmul_11(uint8_t val) { return gmul_2(gmul_2(gmul_2(val)) ^ val) ^ val; }
uint8_t gmul_13(uint8_t val) { return gmul_2(gmul_2(gmul_2(val) ^ val)) ^ val; }
uint8_t gmul_14(uint8_t val) { return gmul_2(gmul_2(gmul_2(val) ^ val) ^ val); }

/*
Mix columns:
Here we multiply the column vector of the state matrix with the following
matrix: [combiation matrix]
2 | 3 | 1 | 1
1 | 2 | 3 | 1
1 | 1 | 2 | 3
2 | 1 | 1 | 2

if the state matrix is:
s0  | s1  | s2  | s3
s4  | s5  | s6  | s7
s8  | s9  | s10 | s11
s12 | s13 | s14 | s15

w1 | w2 | w3 | w4 <- columns

Then the multiplication would look something like this:
2 x w1 + 3 x w2 + 1 x w3 + 1 x w4
1 x w1 + 2 x w2 + 3 x w3 + 1 x w4
1 x w1 + 1 x w2 + 2 x w3 + 3 x w4
3 x w1 + 1 x w2 + 1 x w3 + 2 x w4

The multiplication and addition performed here are in the Galois field (2^8)
therefore the addition is the xor operation
*/

void mixColumn(uint8_t *column) {
  uint8_t copy[4];
  // copying the column vector
  for (int i = 0; i < 4; i++) {
    copy[i] = column[i];
  }

  column[0] = gmul_2(copy[0]) ^ gmul_3(copy[1]) ^ copy[2] ^ copy[3];
  column[1] = gmul_2(copy[1]) ^ gmul_3(copy[2]) ^ copy[3] ^ copy[0];
  column[2] = gmul_2(copy[2]) ^ gmul_3(copy[3]) ^ copy[0] ^ copy[1];
  column[3] = gmul_2(copy[3]) ^ gmul_3(copy[0]) ^ copy[1] ^ copy[2];
}

void mixColumns(uint8_t *state) {
  uint8_t column[4];
  // iterating over the 4 columns
  for (int i = 0; i < 4; i++) {
    // interating over rows of each column to create the column matrix
    for (int j = 0; j < 4; j++) {
      column[j] = state[(j * 4) + i];
    }

    mixColumn(column);

    // putting the values back to state matrix
    for (int j = 0; j < 4; j++) {
      state[(j * 4) + i] = column[j];
    }
  }
}

// Inverse mix columns
void invMixColumn(uint8_t *column) {
  uint8_t copy[4];
  for (int i = 0; i < 4; i++) {
    copy[i] = column[i];
  }

  column[0] =
      gmul_14(copy[0]) ^ gmul_11(copy[1]) ^ gmul_13(copy[2]) ^ gmul_9(copy[3]);
  column[1] =
      gmul_9(copy[0]) ^ gmul_14(copy[1]) ^ gmul_11(copy[2]) ^ gmul_13(copy[3]);
  column[2] =
      gmul_13(copy[0]) ^ gmul_9(copy[1]) ^ gmul_14(copy[2]) ^ gmul_11(copy[3]);
  column[3] =
      gmul_11(copy[0]) ^ gmul_13(copy[1]) ^ gmul_9(copy[2]) ^ gmul_14(copy[3]);
}

void invMixColumns(uint8_t *state) {
  uint8_t column[4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      column[j] = state[(j * 4) + i];
    }
    invMixColumn(column);
    for (int j = 0; j < 4; j++) {
      state[(j * 4) + i] = column[j];
    }
  }
}

void finalRoundAES(uint8_t *state, const uint32_t *roundKey) {
  // final round of AES is same as every other round except we remove the
  // mixColumn part from it
  substituteBytes(state);
  shiftRows(state);
  addRoundKey(state, roundKey);
}

void encryptionAES(uint8_t *state, const uint32_t *expandedKeys) {
  int round;
  // initial transformation
  addRoundKey(state, expandedKeys);

  // main rounds
  // there are 10 rounds in aes-128
  for (round = 1; round < 10; round++) {
    substituteBytes(state);
    shiftRows(state);
    mixColumns(state);

    addRoundKey(state, expandedKeys + (round * 4));
  }

  // final round of aes
  finalRoundAES(state, expandedKeys + (10 * 4));
}

void decryptionAES(uint8_t *state, const uint32_t *expandedKeys) {
  int round;
  addRoundKey(state, expandedKeys + (10 * 4));

  for (round = 9; round > 0; round--) {
    invShiftRows(state);
    invSubstituteBytes(state);
    addRoundKey(state, expandedKeys + (round * 4));
    invMixColumns(state);
  }

  // final round
  invShiftRows(state);
  invSubstituteBytes(state);
  addRoundKey(state, expandedKeys);
}

void padBuffer(char *buffer, int *size) {
  int padLength = AES_BLOCK_SIZE - (*size % AES_BLOCK_SIZE);
  for (int i = 0; i < padLength; i++) {
    buffer[*size + 1] = padLength;
  }
  *size += padLength;
}

char *readFile(const char *filePath, int *fileSize) {
  FILE *fp = fopen(filePath, "rb");
  if (fp == NULL) {
    printf("could not open file %s\n", filePath);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  *fileSize = ftell(fp);
  rewind(fp);

  char *buffer = (char *)malloc((*fileSize + AES_BLOCK_SIZE) * sizeof(char));
  if (buffer == NULL) {
    printf("Memory error\n");
    fclose(fp);
    return NULL;
  }

  fread(buffer, 1, *fileSize, fp);
  fclose(fp);

  padBuffer(buffer, fileSize);
  return buffer;
}

void encryptFile(const char *inputPath, const char *outputPath,
                 const uint8_t *key) {
  int fileSize;
  char *buffer = readFile(inputPath, &fileSize);
  if (buffer == NULL) {
    return;
  }

  uint32_t expandedKeys[44];
  keyExpansion(key, expandedKeys);

  for (int i = 0; i < fileSize; i += AES_BLOCK_SIZE) {
    encryptionAES((uint8_t *)buffer + i, expandedKeys);
  }

  FILE *outputFile = fopen(outputPath, "wb");
  if (outputFile == NULL) {
    printf("Could not open output file %s\n", outputPath);
    free(buffer);
    return;
  }

  fwrite(buffer, 1, fileSize, outputFile);
  fclose(outputFile);
  free(buffer);
}

void decryptFile(const char *inputPath, const char *outputPath,
                 const uint8_t *key) {
  int fileSize;
  char *buffer = readFile(inputPath, &fileSize);
  if (buffer == NULL) {
    return;
  }

  uint32_t expandedKeys[44];
  keyExpansion(key, expandedKeys);

  for (int i = 0; i < fileSize; i += AES_BLOCK_SIZE) {
    decryptionAES((uint8_t *)buffer + i, expandedKeys);
  }

  FILE *outputFile = fopen(outputPath, "wb");
  if (outputFile == NULL) {
    printf("Could not open output file %s\n", outputPath);
    free(buffer);
    return;
  }

  fwrite(buffer, 1, fileSize, outputFile);
  fclose(outputFile);
  free(buffer);
}

int main() {
  uint8_t key[16];
  generatingAesKey(key, sizeof(key));
  /**/
  const char *inputFilePath = "./example.txt";
  const char *encryptedFilePath = "./encrypted.enc";
  const char *decryptedFilePath = "./decrypted.txt";

  encryptFile(inputFilePath, encryptedFilePath, key);
  decryptFile(encryptedFilePath, decryptedFilePath, key);

  /*uint8_t state[4][4] = {{0x32, 0x43, 0xf6, 0xa8},*/
  /*                       {0x88, 0x5a, 0x30, 0x8d},*/
  /*                       {0x31, 0x31, 0x98, 0xa2},*/
  /*                       {0xe0, 0x37, 0x07, 0x34}};*/
  /*uint32_t expandedKeys[44];*/
  /*keyExpansion(key, expandedKeys);*/
  /**/
  /*for (int i = 0; i < 4; i++) {*/
  /*  for (int j = 0; j < 4; j++) {*/
  /*    printf("%d ", state[i][j]);*/
  /*  }*/
  /*  printf("\n");*/
  /*}*/
  /**/
  /*printf("\n");*/
  /**/
  /*encryptionAES((uint8_t *)state, expandedKeys);*/
  /*decryptionAES((uint8_t *)state, expandedKeys);*/
  /**/
  /*for (int i = 0; i < 4; i++) {*/
  /*  for (int j = 0; j < 4; j++) {*/
  /*    printf("%d ", state[i][j]);*/
  /*  }*/
  /*  printf("\n");*/
  /*}*/
  return 0;
}
