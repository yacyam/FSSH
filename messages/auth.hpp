#ifndef AUTH_HPP
#define AUTH_HPP

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
  std::unique_ptr<char[]> pwd; // test yourself: why doesn't the client hash before sending?
  size_t len_pwd;

  SessionRequest(enum connection_t connection, size_t uid, 
                 std::unique_ptr<char[]> pwd, size_t len_pwd);

  Bytes marshal();
  static SessionRequest unmarshal(Bytes bytes);
};

class SessionReply : public Message<SessionReply> {
public:
  enum connection_t connection;
  size_t uid;
  bool success;

  SessionReply(connection_t connection, size_t uid, bool success);

  Bytes marshal();
  static SessionReply unmarshal(Bytes bytes);
};

#endif // AUTH_HPP