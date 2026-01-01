#include <map>
#include <mutex>
#include "app.hpp"
#include "session.hpp"

// maps UID to password
std::map<size_t, std::unique_ptr<char[]>> registered_users{};

bool pwdMatch(char *pwd1, char *pwd2, size_t len) {
  return memcmp(pwd1, pwd2, len) == 0;
}

void sendSessionReply(int fd_conn, SessionRequest request, bool success) {
  sendgeneric(fd_conn, SessionReply{request.connection, request.uid, success}.marshal());
}

void Session::handleRegister(int fd_conn, SessionRequest request) {
  assert(request.connection == connection_register);
  
  if (registered_users.count(request.uid) > 0) {
    std::cout << "Session::handleRegister: uid " << request.uid << " exists" << std::endl;
    sendSessionReply(fd_conn, std::move(request), false);
    return;
  }

  registered_users[request.uid] = std::move(request.pwd);
  sendSessionReply(fd_conn, std::move(request), true);
}

void beginAppProcessing(int fd_conn, size_t uid) {
  assert(registered_users.count(uid) > 0);

  while (true) {
    ShellRequest appRequest = ShellRequest::unmarshal(receivegeneric(fd_conn));
    sendgeneric(fd_conn, appRequest.exec().marshal());
  }
}

void Session::handleLogin(int fd_conn, SessionRequest request) {
  assert(request.connection == connection_login);
  size_t uid = request.uid;
  
  if (registered_users.count(uid) == 0) {
    std::cout << "Session::handleLogin: uid " << request.uid << " does not exist" << std::endl;
    sendSessionReply(fd_conn, std::move(request), false);
    return;
  }

  if (!pwdMatch(registered_users[uid].get(), request.pwd.get(), request.len_pwd)) {
    std::cout << "Session::handleLogin: password mismatch" << std::endl;
    sendSessionReply(fd_conn, std::move(request), false);
    return;
  }

  sendSessionReply(fd_conn, std::move(request), true);
  beginAppProcessing(fd_conn, uid);
}