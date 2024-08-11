#include "../include/UDPSend.h"
int main() {
  char message[1024];
  sprintf(message,
          "--------------------------M_SEARCH--------------------------\n"
          "M-SEARCH * HTTP/1.1\nuuid:%s\n"
          "Man: ssdp:discover\nST: seamless:devices_all\nMX: 5",
          "1233232");

  sendUDP(message, sizeof(message), "1900", "239.255.255.250", 3.0);
}
