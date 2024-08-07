#include "../include/UDPSend.h"
#include "./headerConfig.c"

void joinSSDP(char *myIp, double timer) {
  char *PORT = "1900";
  char *IP = "239.255.255.250";
  char message[1024];
  sprintf(message,
          "--------------------------M_SEARCH--------------------------\n"
          "M-SEARCH * HTTP/1.1\nuuid:%s\n"
          "Man: ssdp:discover\nST: seamless:devices_all\nMX: 5",
          myIp);

  printf("%ld", sizeof(message));

  sendUDP(message, sizeof(message), PORT, IP, timer);
  return;
}
