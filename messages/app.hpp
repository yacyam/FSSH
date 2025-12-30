#include <iostream>
#include <cassert>
#include "msg.hpp"

class ShellReply : public Message<ShellReply> {
public:
  std::unique_ptr<char[]> result;

  ShellReply(std::unique_ptr<char[]> result) : result(std::move(result)) {}

  Bytes marshal() {
    assert(false);
  }

  static ShellReply unmarshal(std::unique_ptr<char[]> data_len_prepended) {
    assert(false);
  }
};

class ShellRequest : public Message<ShellRequest> {
public:
  std::unique_ptr<char[]> command;

  ShellRequest(std::unique_ptr<char[]> command) : command(std::move(command)) {}

  ShellReply exec() {
    assert(false);
    return ShellReply(std::make_unique<char[]>(5));
  }

  Bytes marshal() {
    std::unique_ptr<char[]> msg = std::make_unique<char[]>(sizeof(size_t) + strlen(command.get()));
    mem
    memcpy(msg.get(), command.get(), strlen(command.get()));
    return Bytes(std::move(msg));
  }

  static ShellRequest unmarshal(std::unique_ptr<char[]> data_len_prepended) {
    assert(false);
  }
};
