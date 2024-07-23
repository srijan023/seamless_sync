#include "../include/UDPErrorHandle.h"
#include "../include/customDataTypes.h"
#include "headerConfig.c"

struct customAddInfo findMyIP() {
  struct customAddInfo ipInfo;

#ifdef _WIN32
  WSADATA wsaData;
  // Initialize Winsock2.2
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    handleUDPError("WSAStartup failed", &ipInfo);
    return ipInfo;
  }
#endif
  // this does not require to a a google DNS server
  const char *google_DNS_server = "8.8.8.8";
  const int dns_port = 53;

  // sockaddr_in is a child type data format of sockaddr for ipv4
  struct sockaddr_in serv, clnt;
  // creating a UDP socket
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  // error handling
  if (sock < 0) {
    handleUDPError("Could not create a UDP socket", &ipInfo);
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
    handleUDPError("Could not connect the socket", &ipInfo);
#ifdef _WIN32
    WSACleanup();
    closesocket(sock);
#elif __linux__
    close(sock);
#endif
    return ipInfo;
  }

  socklen_t clntnamelen = sizeof(clnt);
  err = getsockname(sock, (struct sockaddr *)&clnt, &clntnamelen);
  if (err == -1) {
    handleUDPError("Could not get the socket name", &ipInfo);
#ifdef _WIN32
    WSACleanup();
    closesocket(sock);
#elif __linux__
    close(sock);
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
    handleUDPError("Couldn't get the IP address", &ipInfo);
#ifdef _WIN32
    WSACleanup();
    closesocket(sock);
#elif __linux__
    close(sock);
#endif
    return ipInfo;
  }
}
