#include "../src/headerConfig.c"
#include "../include/UDPSend.h"
#include "../include/customDataTypes.h"

int main() {
    char* message = "This is a new message";
    struct customAddInfo info = sendUDP(message, strlen(message), "12345", "127.0.0.1", 0);
    printf("%s\n", info.message);
    return 0;
}