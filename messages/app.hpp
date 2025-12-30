#include <iostream>
#include <cassert>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg.hpp"

#define PIPE_READ 0
#define PIPE_WRITE 1
#define BUF_SIZ 65536

class ShellReply : public Message<ShellReply> {
public:
  std::unique_ptr<char[]> result;
  size_t len;

  ShellReply(std::unique_ptr<char[]> result, size_t len);
  
  Bytes marshal();
  static ShellReply unmarshal(Bytes bytes);
};

class ShellRequest : public Message<ShellRequest> {
public:
  std::unique_ptr<char[]> command;
  size_t len;

  ShellRequest(std::unique_ptr<char[]> command, size_t len);
  ShellReply exec();

  Bytes marshal();
  static ShellRequest unmarshal(Bytes bytes);
};
