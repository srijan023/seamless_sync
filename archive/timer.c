#include <stdio.h>
#include <time.h>
#include <unistd.h>
int main() {
  time_t currTime = time(NULL);
  printf("%ld\n", currTime);

  sleep(10);

  time_t newTime = time(NULL);
  printf("%ld", newTime - currTime);
  return 0;
}
