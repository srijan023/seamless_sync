#include "../src/headerConfig.c"
#include "../include/UDPSend.h"

typedef struct customAddInfo SSDPSendMessage;
int main() {
    char* group = "239.255.255.250";
    char* port = "1900";

    char* message = "hello world";
    // format: message, message length, port: 1900 for SSDP, 
    //ip: "239.255.255.250" for SSDP and sendMultiple? 0 -> false 1-> true
    SSDPSendMessage msg = sendUDP(message, strlen(message), port, group, 1);
    printf("%s\n", msg.message);

    return 0;
}