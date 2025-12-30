#include <iostream>
#include "msg.hpp"

class SKC {
  char key; // makes things easier for now
public:
  SKC(char key);
  Bytes encrypt(Bytes bytes);
  Bytes decrypt(Bytes bytes);
};