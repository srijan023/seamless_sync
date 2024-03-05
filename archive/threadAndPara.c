#include <stdio.h>
#include <pthread.h>
#include <windows.h>

void* helloworld(void* arg) {
    int* condition = arg;
    int* count = malloc(sizeof(int));
    *count = 0;
    while (*condition) {
        (*count)++;
        Sleep(1000);
        printf("Hello world\n");
    }
    return (void**)count;
}

int main() {
    pthread_t newThread;

    int condition = 1;
    pthread_create(&newThread, NULL, helloworld, &condition);
    Sleep(2000);
    condition = 0;
    int* returnValue;
    pthread_join(newThread, (void**)&returnValue);
    printf("%d", (*returnValue));
    free(returnValue);
    return 0;
}