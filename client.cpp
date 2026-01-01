#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "auth.hpp"
#include "app.hpp"
#include "sym.hpp"

#define SERVER_PORT 5432
#define MAX_LINE 256

#define IP_SERVER "127.0.0.1"
#define PROT_IP 0

void remove_newline(char *buf) {
  for(int i = 0;;i++) {
    if (buf[i] == '\r' || buf[i] == '\n') {
      buf[i] = '\0';
      return;
    }
  }

}

int establish_connection() {
  struct sockaddr_in sin;
  int fd_sock;

  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  inet_aton(IP_SERVER, &sin.sin_addr);
  sin.sin_port = htons(SERVER_PORT);

  if ((fd_sock = socket(PF_INET, SOCK_STREAM, PROT_IP)) < 0) {
      std::cout << "establish_connection: socket error" << std::endl;
      exit(1);
  }
  if (connect(fd_sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
      std::cout << "establish_connection: connect error" << std::endl;
      std::terminate();
  }
  return fd_sock;
}

void handle_register() {
  char buf[MAX_LINE];

  // read the username
  fputs("user id: ", stdout);
  fgets(buf, MAX_LINE, stdin);
  size_t uid = atol(buf);

  // read the password
  std::unique_ptr<char[]> pwd = std::make_unique<char[]>(MAX_LINE);
  fputs("password: ", stdout);
  fgets(pwd.get(), MAX_LINE, stdin);
  remove_newline(pwd.get());
  size_t len_pwd = strlen(pwd.get());

  // send register request
  int fd_sock = establish_connection();
  SessionRequest sessionRequest{connection_register, uid, std::move(pwd), len_pwd};
  sendgeneric(fd_sock, sessionRequest.marshal());
}

void handle_login() {
  std::cout << "login unimplemented" << std::endl;
  std::terminate();
}

int main(int argc, char * argv[]) {
  char buf[MAX_LINE];

  fputs("command: ", stdout);
  while (fgets(buf, MAX_LINE, stdin) != NULL) {
    switch (buf[0]) {
    case 'R':
      handle_register();
      break;
    case 'L':
      handle_login();
      break;
    case 'Q':
      std::terminate();
      break;
    default:
      std::cout << "Unknown command type" << std::endl;
      break;
    }
    fputs("command: ", stdout);
  }
}
