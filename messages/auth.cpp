#include <iostream>
#include <cassert>
#include "msg.hpp"
#include "auth.hpp"

SessionRequest::SessionRequest(enum connection_t connection, size_t uid, 
    std::unique_ptr<char[]> pwd, size_t len_pwd) : connection(connection), uid(uid),
                                                   pwd(std::move(pwd)), len_pwd(len_pwd) {}

Bytes SessionRequest::marshal() {
  size_t total_bytes{sizeof(connection) + sizeof(uid) + sizeof(len_pwd) + len_pwd};

  std::unique_ptr<char[]> data = std::make_unique<char[]>(total_bytes);
  char *p = data.get();

  memcpy(p, &connection, sizeof(connection));
  p += sizeof(connection);
  memcpy(p, &uid, sizeof(uid));
  p += sizeof(uid);
  memcpy(p, &len_pwd, sizeof(len_pwd));
  p += sizeof(len_pwd);
  memcpy(p, pwd.get(), len_pwd);

  return Bytes(std::move(data), total_bytes);
}

SessionRequest SessionRequest::unmarshal(Bytes bytes) {
  assert(bytes.len >= (sizeof(uid) + sizeof(connection) + sizeof(len_pwd)));
  enum connection_t connection_incoming;
  size_t uid_incoming, len_pwd_incoming;

  char *p = bytes.data.get();
  memcpy(&connection_incoming, p, sizeof(connection_incoming));
  p += sizeof(connection_incoming);
  memcpy(&uid_incoming, p, sizeof(uid_incoming));
  p += sizeof(uid_incoming);
  memcpy(&len_pwd_incoming, p, sizeof(len_pwd_incoming));
  p += sizeof(len_pwd_incoming);

  std::unique_ptr<char[]> pwd = std::make_unique<char[]>(len_pwd_incoming);
  memcpy(pwd.get(), p, len_pwd_incoming);

  return SessionRequest(connection_incoming, uid_incoming, std::move(pwd), len_pwd_incoming);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SessionReply::SessionReply(connection_t connection, size_t uid, bool success) :
  connection(connection), uid(uid), success(success) {}


Bytes SessionReply::marshal() {
  size_t total_bytes{sizeof(connection) + sizeof(uid) + sizeof(success)};

  std::unique_ptr<char[]> data = std::make_unique<char[]>(total_bytes);
  char *p = data.get();

  memcpy(p, &connection, sizeof(connection));
  p += sizeof(connection);
  memcpy(p, &uid, sizeof(uid));
  p += sizeof(uid);
  memcpy(p, &success, sizeof(success));

  return Bytes(std::move(data), total_bytes);
}


SessionReply SessionReply::unmarshal(Bytes bytes) {
  assert(bytes.len == (sizeof(uid) + sizeof(connection) + sizeof(success)));
  enum connection_t connection_incoming;
  size_t uid_incoming;
  bool success_incoming;

  char *p = bytes.data.get();
  memcpy(&connection_incoming, p, sizeof(connection_incoming));
  p += sizeof(connection_incoming);
  memcpy(&uid_incoming, p, sizeof(uid_incoming));
  p += sizeof(uid_incoming);
  memcpy(&success_incoming, p, sizeof(success_incoming));

  return SessionReply(connection_incoming, uid_incoming, success_incoming);
}