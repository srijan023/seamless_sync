#include "src/headerConfig.c"
#include "include/customDataTypes.h"
#include "src/UDPSend.c"
// #include "src/UDPListen.c"
#include "src/getMyIp.c"

typedef struct customAddInfo customAdd;
int main() {
    char* message = "This is the message from main";
    // while (1) {
    customAdd result = sendUDP(message, strlen(message), "1900", "239.255.255.250", 0);
    // customAdd result = listenUDP("12345", "127.0.0.1");
    // customAdd result = findMyIP();
    printf("%s", result.message);
    // }
    return 0;
}