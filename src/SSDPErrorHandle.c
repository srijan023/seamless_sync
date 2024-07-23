#include "../include/customDataTypes.h"
#include "headerConfig.c"

void handleSSDPError(char *msg, struct ssdpMessage *ssdpMsg) {
  ssdpMsg->size = -1;
  ssdpMsg->arr = NULL;
  strcpy(ssdpMsg->message, msg);
  return;
}
