#include "../include/customDataTypes.h"
#include <pthread.h>
#include "../src/headerConfig.c"
#include  "./SSDPListenerConnection.c"

// void* SSDPReceiver(void* args) {
//     struct customSSDPThread* threadMsg = args;
//     char msgBuffer[256];
//     int addrlen = sizeof(threadMsg->addr);
//     int count = 0;
//     while (*(threadMsg->condition)) {
//         printf("%d", *(threadMsg->condition));
//         int nbytes = recvfrom(threadMsg->socket, msgBuffer, 256, 0, (struct sockaddr*)&threadMsg->addr, &addrlen);
//         if (nbytes > 0) {
//             count++;
//             msgBuffer[nbytes] = '\0';
//             printf("%s\n", msgBuffer);
//         }
//     }
//     struct customAddInfo* SSDPThreadReply = malloc(sizeof(struct customAddInfo));
//     sprintf(SSDPThreadReply->message, "%d messages were receieved\n", count);
//     SSDPThreadReply->status = 0;
//     return SSDPThreadReply;
// }

// Listner handler
int main() {
    pthread_t SSDPThread;
    int doLooping = 1;
    pthread_create(&SSDPThread, NULL, SSDPListen, &doLooping);
    Sleep(10000); // suspend
    doLooping = 0;
    struct customAddInfo* revMsg;
    pthread_join(SSDPThread, (void**)&revMsg);
    printf("here\n");
    printf("%s\n", revMsg->message);
    free(revMsg);
    return 0;
}