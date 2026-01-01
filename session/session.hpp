#include "msg.hpp"
#include "auth.hpp"

class Session {
public:
  Session() = delete; // static class

  static void handleRegister(int fd_conn, SessionRequest request);
  static void handleLogin(int fd_conn, SessionRequest request);
};
