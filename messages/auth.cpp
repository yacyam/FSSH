#include <iostream>
#include <cassert>
#include "msg.hpp"
#include "auth.hpp"

SessionRequest::SessionRequest(size_t uid) : uid(uid) {}

Bytes SessionRequest::marshal() {
  std::unique_ptr<char[]> data = std::make_unique<char[]>(sizeof(uid));
  memcpy(data.get(), &uid, sizeof(uid));
  return Bytes(std::move(data), sizeof(uid));
}

SessionRequest SessionRequest::unmarshal(Bytes bytes) {
  assert(bytes.len == sizeof(uid));
  return SessionRequest(*((size_t*)bytes.data.get()));
}
