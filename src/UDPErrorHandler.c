#include "../include/customDataTypes.h"
#include "headerConfig.c"

void handleUDPError(char *msg, struct customAddInfo *info) {
  info->status = -1;
  strcpy(info->message, msg);
  return;
}
