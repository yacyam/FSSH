#include "sym.hpp"

SKC::SKC(char key) : key(key) {}

Bytes _xor(Bytes bytes, char key) {
  std::unique_ptr<char[]> out = std::make_unique<char[]>(bytes.len);

  for (size_t idx = 0; idx < bytes.len; idx++) {
    out[idx] = (char)((size_t)bytes.data[idx] ^ key);
  }

  return Bytes(std::move(out), bytes.len);
}

Bytes SKC::encrypt(Bytes bytes) {
  return _xor(std::move(bytes), key);
}

Bytes SKC::decrypt(Bytes bytes) {
  return _xor(std::move(bytes), key);
}