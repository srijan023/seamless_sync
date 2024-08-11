#include "../include/SSDPListenerConnection.h"
#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include <pthread.h>

// void* SSDPReceiver(void* args) {
//     struct customSSDPThread* threadMsg = args;
//     char msgBuffer[256];
//     int addrlen = sizeof(threadMsg->addr);
//     int count = 0;
//     while (*(threadMsg->condition)) {
//         printf("%d", *(threadMsg->condition));
//         int nbytes = recvfrom(threadMsg->socket, msgBuffer, 256, 0, (struct
//         sockaddr*)&threadMsg->addr, &addrlen); if (nbytes > 0) {
//             count++;
//             msgBuffer[nbytes] = '\0';
//             printf("%s\n", msgBuffer);
//         }
//     }
//     struct customAddInfo* SSDPThreadReply = malloc(sizeof(struct
//     customAddInfo)); sprintf(SSDPThreadReply->message, "%d messages were
//     receieved\n", count); SSDPThreadReply->status = 0; return
//     SSDPThreadReply;
// }

// Listner handler
int main() {
  pthread_t SSDPThread;
  int doLooping = 1;
  pthread_create(&SSDPThread, NULL, SSDPListen, NULL);
  sleep(10); // suspend
  doLooping = 0;
  struct ssdpMessage *revMsg;
  pthread_join(SSDPThread, (void **)&revMsg);
  printf("%s\n", revMsg->message);
  printf("%d\n", revMsg->size);
  free(revMsg->arr);
  free(revMsg);
  return 0;
}
