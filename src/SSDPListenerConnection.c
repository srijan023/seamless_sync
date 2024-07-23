#include "../include/SSDPErrorHandle.h"
#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"
#include <pthread.h>
#include <sys/socket.h>

int isPresent(char **arr, char *key, int size) {
  for (int i = 0; i < size; i++) {
    if (strcmp(arr[i], key) == 0) {
      return 1;
    }
  }
  return 0;
}

char **doubleArraySize(char **arr, int size, int *newSize) {
  *newSize = size * 2;
  char **newArr = realloc(arr, (*newSize) * sizeof(char *));
  if (newArr == NULL) {
    perror("Failed to reallocate memory");
    exit(EXIT_FAILURE);
  }
  return newArr;
}

void *SSDPListen(void *arg) {
  struct ssdpMessage *msg =
      (struct ssdpMessage *)malloc(sizeof(struct ssdpMessage));
#ifdef _WIN32
  WSADATA wsaData;
  // Initialize Winsock2.2
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    handleUDPError("WSAStartup failed", msg);
    return (void *)msg;
  }
#endif

  int SSDPListener = socket(AF_INET, SOCK_DGRAM, 0);
  if (SSDPListener < 0) {
    handleSSDPError("Socket couldn't be created!\n", msg);
#ifdef _WIN32
    WSACleanup();
#endif
    return (void *)msg;
  }

  struct sockaddr_in theirAddrs;
  memset(&theirAddrs, 0, sizeof(theirAddrs));
  theirAddrs.sin_family = AF_INET;
  theirAddrs.sin_addr.s_addr = htonl(INADDR_ANY); // any interface
  theirAddrs.sin_port = htons(1900);              // specifically for SSDP

  if (bind(SSDPListener, (struct sockaddr *)&theirAddrs, sizeof(theirAddrs)) <
      0) {
    handleSSDPError("Socket binding failed!\n", msg);
#ifdef _WIN32
    closesocket(SSDPListener);
    WSACleanup();
#elif __linux__
    close(SSDPListener);
#endif
    return (void *)msg;
  }

  // joining the SSDP pool
  struct ip_mreq multi_group;
  multi_group.imr_interface.s_addr = htonl(INADDR_ANY);
  multi_group.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
  if (setsockopt(SSDPListener, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                 (char *)&multi_group, sizeof(multi_group)) < 0) {
    handleSSDPError("Can't join the multicast group!\n", msg);
#ifdef _WIN32
    closesocket(SSDPListener);
    WSACleanup();
#elif __linux__
    close(SSDPListener);
#endif
    return (void *)msg;
  }

  char msgBuffer[256];
  int addrlen = sizeof(theirAddrs);
  int count = -1;
  const char *key = "uuid:";
  int *condition = (int *)arg;

  // initializing a multi dimensional dynamic array
  int size = 10;
  // allocating the outer index array
  char **IpList = (char **)malloc(size * sizeof(char *));
  if (IpList == NULL) {
    handleSSDPError("Can't create a dynamic array", msg);
#ifdef _WIN32
    closesocket(SSDPListener);
    WSACleanup();
#elif __linux__
    close(SSDPListener);
#endif
    return (void *)msg;
  }

  // allocating the inner array to store ip-address
  for (int i = 0; i < size; i++) {
    IpList[i] = malloc(20 * sizeof(char));
    if (IpList[i] == NULL) {
      handleSSDPError("Can't create a dynamic array", msg);
#ifdef _WIN32
      closesocket(SSDPListener);
      WSACleanup();
#elif __linux__
      close(SSDPListener);
#endif
      return (void *)msg;
    }
  }

  // while true send SSDP messages
  while (*condition) {
    int nbytes =
        recvfrom(SSDPListener, msgBuffer, 256, 0,
                 (struct sockaddr *)&theirAddrs, (socklen_t *)&addrlen);
    if (nbytes > 0) {
      msgBuffer[nbytes] = '\0';
      // checking if seamless is in the SSDP request header
      const char *seam = strstr(msgBuffer, "seamless");
      if (seam) {
        const char *ipStart = strstr(msgBuffer, key);
        if (ipStart) {
          ipStart += strlen(key);
          char ip[16]; // 255.255.255.255
          sscanf(ipStart, "%15s", ip);
          printf("%s\n", ip);
          if (isPresent(IpList, ip, count + 1)) {
            continue;
          }
          count++;
          strncpy(IpList[count], ip, 20);
          if (count == size - 1) {
            IpList = doubleArraySize(IpList, size, &size);
            for (int i = count; i < size; i++) {
              IpList[i] = (char *)malloc(size * 20);
              if (IpList[i] == NULL) {
                handleSSDPError("Can't create a dynamic array", msg);
#ifdef _WIN32
                closesocket(SSDPListener);
                WSACleanup();
#elif __linux__
                close(SSDPListener);
#endif
                return (void *)msg;
              }
            }
          }
        } else {
          // uuid not found
          handleSSDPError("uuid not found on header", msg);
#ifdef _WIN32
          closesocket(SSDPListener);
          WSACleanup();
#elif __linux__
          close(SSDPListener);
#endif
          return (void *)msg;
        }
      }
    }
  }
  msg->size = count + 1;
  msg->arr = IpList;
  strcpy(msg->message, "Ip address list inserted successfully");
#ifdef _WIN32
  closesocket(SSDPListener);
  WSACleanup();
#elif __linux__
  close(SSDPListener);
#endif
  return (void *)msg;
}
