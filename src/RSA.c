#include "./headerConfig.c"

// Function to calculate the greatest common divisor
long long gcd(long long a, long long b) {
  while (b != 0) {
    long long temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

// Extended Euclidean Algorithm to find modular inverse
long long extended_gcd(long long a, long long b, long long *x, long long *y) {
  if (b == 0) {
    *x = 1;
    *y = 0;
    return a;
  }
  long long x1, y1;
  long long gcd = extended_gcd(b, a % b, &x1, &y1);
  *x = y1;
  *y = x1 - y1 * (a / b);
  return gcd;
}

// Function to find modular inverse of a under modulo m
long long modInverse(long long a, long long m) {
  long long x, y;
  long long g = extended_gcd(a, m, &x, &y);
  if (g != 1) {
    printf("No modular inverse exists for %lld modulo %lld\n", a, m);
    return -1;
  } else {
    return (x % m + m) % m;
  }
}

// Function to perform modular exponentiation
// It returns (base^exp) % mod
long long modularExponentiation(long long base, long long exp, long long mod) {
  long long result = 1;
  base = base % mod;
  while (exp > 0) {
    if (exp % 2 == 1) {
      result = (result * base) % mod;
    }
    exp = exp >> 1;
    base = (base * base) % mod;
  }
  return result;
}

// Function to check if a number is prime
int isPrime(long long num) {
  // handling base cases
  if (num < 2)
    return 0;
  if (num < 4)
    return 1;
  if (num % 2 == 0 || num % 3 == 0)
    return 0;

  for (long long i = 5; i * i <= num; i += 6) {
    if (num % i == 0 || num % (i + 2) == 0)
      return 0;
  }
  return 1;
}

// Function to generate a large prime number
long long generatePrime(int bitLength) {
  long long lower = 1L << (bitLength - 1);
  long long upper = (1L << bitLength) - 1;
  long long num;
  do {
    num = (rand() % (upper - lower + 1)) + lower;
  } while (!isPrime(num));
  return num;
}

// Function to generate RSA keys
long long generateRSAKeys(long long bitLength, long long *e, long long *d,
                          long long *n) {
  long long p = generatePrime(bitLength / 2);
  long long q = generatePrime(bitLength / 2);
  *n = p * q;
  long long phi = (p - 1) * (q - 1);

  // Choose e such that 1 < e < phi and gcd(e, phi) = 1
  do {
    *e = rand() % (phi - 2) + 2;
  } while (gcd(*e, phi) != 1);

  // Calculate d, the modular inverse of e mod phi
  *d = modInverse(*e, phi);
  return phi;
}

// Function to encrypt a message using RSA
long long rsaEncrypt(long long message, long long e, long long n) {
  return modularExponentiation(message, e, n);
}

// Function to decrypt a message using RSA
long long rsaDecrypt(long long cipher, long long d, long long n) {
  return modularExponentiation(cipher, d, n);
}

// int main() {
//   // Initialize random seed
//   srand(time(NULL));
//
//   // Key generation
//   long long e, d, n;
//   int bitLength = 30; // Adjust bit length as needed
//   long long phi = generateRSAKeys(bitLength, &e, &d, &n);
//
//   printf("Public Key: (e = %lld, n = %lld)\n", e, n);
//   printf("Private Key: (d = %lld, n = %lld)\n", d, n);
//
//   // Example message (AES key pattern)
//   long long message[16] = {99, 63,  227, 19,  47,  243, 71, 22,
//                            63, 200, 192, 207, 227, 137, 43, 200};
//
//   printf("Original message: \n");
//   for (int i = 0; i < 16; i++) {
//     printf("%lld\t", message[i]);
//   }
//
//   long long encryptedMessage[16];
//   // Encryption
//   for (int i = 0; i < 16; i++) {
//     encryptedMessage[i] = rsaEncrypt(message[i], e, n);
//   }
//   printf("\nEncrypted message: \n");
//   for (int i = 0; i < 16; i++) {
//     printf("%lld\t", encryptedMessage[i]);
//   }
//
//   long long decryptedMessage[16];
//   // Decryption
//   for (int i = 0; i < 16; i++) {
//     decryptedMessage[i] = rsaDecrypt(encryptedMessage[i], d, n);
//   }
//   printf("\nDecryption message: \n");
//   for (int i = 0; i < 16; i++) {
//     printf("%lld\t", decryptedMessage[i]);
//   }
//
//   return 0;
// }
