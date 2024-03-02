#include "headerconfig.c"
#include "../include/customDataTypes.h"

void handleUDPError(char* msg, struct customAddInfo* info) {
    info->status = -1;
    strcpy(info->message, msg);
    return;
}