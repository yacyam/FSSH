#include <iostream>
#include <cassert>
#include "msg.hpp"

// session authentication protocol:
//  C connects to S (sockets)
//  C -> S: User ID
//  S -> C: Ack/Err

class SessionRequest : public Message<SessionRequest> {
public:
  size_t uid;

  SessionRequest(size_t uid) : uid(uid) {}

  SessionRequest(int fd_sock) { unmarshal(receivegeneric(fd_sock)); }

  Bytes marshal() {
    //                                      length            uid
    char *data_len_prepended = new char[sizeof(size_t) + sizeof(size_t)];
    *((size_t*)data_len_prepended) = sizeof(size_t);
    *(((size_t*)data_len_prepended) + 1) = uid;

    return Bytes(data_len_prepended, sizeof(size_t) + sizeof(size_t));
  }

  static SessionRequest unmarshal(std::unique_ptr<char[]> data_len_prepended) {
    assert(*((size_t*)data_len_prepended.get()) == sizeof(size_t));
    return SessionRequest(*(((size_t*)data_len_prepended.get()) + 1));
  }
};