#include "headerConfig.c"
#include "customDataTypes.h"
// #include "UDPSend.c"
// #include "UDPListen.c"
#include "getMyIp.c"

typedef struct customAddInfo customAdd;
int main() {
    char* message = "This is the message from main";
    // customAdd result = sendUDP(message, strlen(message), "12345", "127.0.0.1");
    // customAdd result = listenUDP("12345", "127.0.0.1");
    customAdd result = findMyIP();
    printf("%s", result.message);
    return 0;
}