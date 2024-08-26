#ifndef CUSTOMDATATYPE_H
#define CUSTOMDATATYPE_H

struct customAddInfo {
  int status;
  char message[1024];
};

struct ssdpMessage {
  char **arr;
  int size;
  char message[1024];
};

struct fileInfo {
  char type;
  char name[50];
  long long size;
};

struct socketInfo {
  int *socket;
  int status;
  char pub_key[50];
};
#endif
