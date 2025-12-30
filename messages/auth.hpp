#include <iostream>
#include <cassert>
#include "msg.hpp"

class SessionRequest : public Message<SessionRequest> {
public:
  size_t uid;

  SessionRequest(size_t uid);

  Bytes marshal();
  static SessionRequest unmarshal(Bytes bytes);
};