#include "../include/UDPListen.h"
#include "../src/headerConfig.c"
#include "../include/customDataTypes.h"

int main() {
    struct customAddInfo message = listenUDP("12345", "127.0.0.1");
    printf("%s\n", message.message);
    return 0;
}