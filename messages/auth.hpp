#include <iostream>
#include <cassert>
#include "msg.hpp"

enum connection_t {
  connection_register,
  connection_login
};

class SessionRequest : public Message<SessionRequest> {
public:
  enum connection_t connection;
  size_t uid;
  std::unique_ptr<char[]> pwd;
  size_t len_pwd;

  SessionRequest(enum connection_t connection, size_t uid, 
                 std::unique_ptr<char[]> pwd, size_t len_pwd);

  Bytes marshal();
  static SessionRequest unmarshal(Bytes bytes);
};