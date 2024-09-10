#include "./headerConfig.c"
#include "KeyStorageGlobal.h"
#include "UDPSend.h"
#include "uiCustomData.h"

long long m_rsa_e;
long long m_rsa_n;

void *joinSSDP(gpointer user_data) {
  JoinSSDPThread *thread_data = (JoinSSDPThread *)user_data;

  char *PORT = "1900";
  char *IP = "239.255.255.250";
  char message[1024];
  // printf("I am sending: %lld, %lld", m_rsa_e, m_rsa_n);
  sprintf(message,
          "--------------------------M_SEARCH--------------------------\n"
          "M-SEARCH * HTTP/1.1\nuuid:%s\npub_e:%lld\npub_n:%lld\n"
          "Man: ssdp:discover\nST: seamless:devices_all\nMX: 5",
          thread_data->myIp, m_rsa_e, m_rsa_n);

  printf("%ld", sizeof(message));

  sendUDP(message, sizeof(message), PORT, IP, thread_data->timer);

  g_free(thread_data);

  return NULL;
}
