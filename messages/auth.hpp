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

  Bytes marshal() {
    std::unique_ptr<char[]> data = std::make_unique<char[]>(sizeof(uid));
    memcpy(data.get(), &uid, sizeof(uid));
    return Bytes(std::move(data), sizeof(uid));
  }

  static SessionRequest unmarshal(Bytes bytes) {
    assert(bytes.len == sizeof(uid));
    return SessionRequest(*((size_t*)bytes.data.get()));
  }
};