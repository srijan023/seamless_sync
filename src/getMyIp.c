#include "../include/customDataTypes.h"
#include "headerConfig.c"

typedef struct customAddInfo customAdd;
void handleError(char *mess, customAdd *info) {
  info->status = -1;
  strcpy(info->message, mess);
}

customAdd findMyIP() {
  customAdd ipInfo;

#ifdef _WIN32
  WSADATA wsaData;
  // Initialize Winsock2.2
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    handleError("WSAStartup failed", &ipInfo);
    return ipInfo;
  }
#endif
  const char *google_DNS_server = "8.8.8.8";
  const int dns_port = 53;

  struct sockaddr_in serv, clnt;
  // creating a UDP socket
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  // error handling
  if (sock < 0) {
    handleError("Could not create a UDP socket", &ipInfo);
#ifdef _WIN32
    WSACleanup();
#endif
    return ipInfo;
  }

  memset(&serv, 0, sizeof(serv));
  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = inet_addr(google_DNS_server);
  serv.sin_port = htons(dns_port);

  int err = connect(sock, (const struct sockaddr *)&serv, sizeof(serv));
  if (err == -1) {
    handleError("Could not connect the socket", &ipInfo);
#ifdef _WIN32
    WSACleanup();
#endif
    return ipInfo;
  }

  socklen_t clntnamelen = sizeof(clnt);
  err = getsockname(sock, (struct sockaddr *)&clnt, &clntnamelen);
  if (err == -1) {
    handleError("Could not get the socket name", &ipInfo);
#ifdef _WIN32
    WSACleanup();
#endif
    return ipInfo;
  }

  char ipAddress[100];
  const char *p = inet_ntop(AF_INET, &clnt.sin_addr, ipAddress, 100);

  if (p != NULL) {
    ipInfo.status = 0;
    strcpy(ipInfo.message, ipAddress);
#ifdef _WIN32
    WSACleanup();
    closesocket(sock);
#elif __linux__
    close(sock);
#endif
    return ipInfo;
  } else {
    handleError("Couldn't get the IP address", &ipInfo);
#ifdef _WIN32
    WSACleanup();
    closesocket(sock);
#elif __linux__
    close(sock);
#endif
    return ipInfo;
  }
}
