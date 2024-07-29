#include "../../include/SSDPListenerConnection.h"
#include <gtk/gtk.h>

struct ssdpMessage doLoopThread() {
  pthread_t SSDPThread;
  // int doLooping = 1;
  pthread_create(&SSDPThread, NULL, SSDPListen, NULL);
  g_usleep(1000000);
  // doLooping = 0;
  struct ssdpMessage *revMsg;
  pthread_join(SSDPThread, (void **)&revMsg);
  return *revMsg;
}
