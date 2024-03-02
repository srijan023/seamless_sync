#include "src/headerConfig.c"
#include "include/customDataTypes.h"
#include "include/UDPSend.h"
// #include "src/UDPListen.c"
#include "include/getMyIp.h"

int main() {
    char* message = "This is the message from main";
    // customAdd result = sendUDP(message, strlen(message), "1900", "239.255.255.250", 0);
    // customAdd result = listenUDP("12345", "127.0.0.1");
    struct customAddInfo result = findMyIP();
    printf("%s", result.message);
    return 0;
}