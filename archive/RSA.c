#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void findUandT(long long num, long long *u, long long *t) {
  *t = 0;
  while (num % 2 == 0) {
    num /= 2;
    (*t)++;
  }
  *u = num;
}

long long modularExponentiation(long long a, long long b, long long n) {
  long long result = 1;
  a = a % n;
  while (b > 0) {
    if (b % 2 == 1) {
      result = (result * a) % n;
    }
    b = b >> 1;
    a = (a * a) % n;
  }
  return result;
}

int witness(long long a, long long num) {
  long long u = 0;
  long long t = 0;
  findUandT(num - 1, &u, &t);
  long long b_init, b_next;
  b_init = modularExponentiation(a, u, num);
  for (int i = 0; i < t; i++) {
    b_next = (b_init * b_init) % num;
    if (b_next == 1 && b_init != 1 && b_init != num - 1) {
      return 1;
    }
    b_init = b_next;
  }
  if (b_next != 1) {
    return 1;
  }
  return 0;
}

// miller-rabin primality check
int isPrime(long long num) {
  if (num % 2 == 0) {
    return 0;
  }
  for (int i = 0; i < 40; i++) {
    long long a = (random() % (num - 2)) + 1;
    if (witness(a, num)) {
      return 0;
    }
  }
  return 1;
}

long long generatePrimeNumber(int bitLength) {
  long long lower = 1L << (bitLength - 1);
  long long upper = (1L << bitLength) - 1;
  long long num;
  do {
    num = (random() % (upper - lower)) + lower;
  } while (!isPrime(num));
  return num;
}

long long gcd(long long a, long long b) {
  while (b != 0) {
    long long t = b;
    b = a % b;
    a = t;
  }
  return a;
}

long long extended_gcd(long long a, long long b, long long *x, long long *y) {
  if (b == 0) {
    *x = 1;
    *y = 0;
    return a;
  }
  long long x1, y1;
  long long d = extended_gcd(b, a % b, &x1, &y1);
  *x = y1;
  *y = x1 - y1 * (a / b);
  return d;
}

long long modInverse(long long a, long long m) {
  long long x, y;
  long long g = extended_gcd(a, m, &x, &y);
  if (g == 1) {
    x = (x % m + m) % m;
  }
  return x;
}

long long generateKeys(long long p, long long q, long long *public,
                       long long *private) {
  long long n = p * q;
  long long phi_n = (p - 1) * (q - 1);

  // choosing the public exponent
  long long e;
  do {
    e = rand() % (phi_n - 2) + 2;
    if (e % 2 == 0) {
      continue;
    }
  } while (gcd(e, phi_n) != 1);

  long long d = modInverse(e, phi_n);

  printf("The public keys are (%lld, %lld)\n", e, n);
  printf("The private keys are (%lld, %lld)\n", d, n);

  *public = e;
  *private = d;

  return n;
}

int main() {
  // setting up the seed for random number generation.
  srand(time(NULL));
  long long prime1 = generatePrimeNumber(30);
  long long prime2 = generatePrimeNumber(30);

  long long e, d;
  long long n = generateKeys(prime1, prime2, &e, &d);

  return 0;
}
