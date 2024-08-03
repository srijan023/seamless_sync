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
  char name[1024];
  char type;
};
#endif
