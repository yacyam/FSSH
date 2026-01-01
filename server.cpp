#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <string.h>
#include "auth.hpp"
#include "app.hpp"
#include "sym.hpp"

#define MAX_PENDING  1
#define MAX_LINE     256

#define PROT_IP 0

class ConnectionAcceptor {
  std::string ip;
  size_t port, max_connections;
  int fd_sock;
public:
  ConnectionAcceptor(std::string ip, size_t port, size_t max_connections) : 
    ip(ip), port(port), max_connections(max_connections) {}

  void handshake(int fd_accepted_conn) {
    SessionRequest sessionRequest = SessionRequest::unmarshal(receivegeneric(fd_accepted_conn));

    switch (sessionRequest.connection) {
    case connection_register:
      std::cout << "ConnectionAcceptor.handshake: Register Unimplemented" << std::endl;
      close(fd_accepted_conn);
      break;
    case connection_login:
      std::cout << "ConnectionAcceptor.handshake: Login Unimplemented" << std::endl;
      close(fd_accepted_conn);
      break;
    default:
      std::cout << "ConnectionAcceptor.handshake: Unknown Connection Type" << std::endl;
      close(fd_accepted_conn);
      break;
    }
  }
  
  void run() {
    struct sockaddr_in sin;
    socklen_t addr_len;
    int enable = 1;

    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip.c_str());
    sin.sin_port = htons(port);

    // setup socket
    if ((fd_sock = socket(PF_INET, SOCK_STREAM, PROT_IP)) < 0) {
      std::cout << "ConnectionAcceptor.loop: socket error" << std::endl;
      exit(1);
    }
    if (setsockopt(fd_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
      std::cout << "ConnectionAcceptor.loop: sockopt(addr) error" << std::endl;
      exit(1);
    }
    if (setsockopt(fd_sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
      std::cout << "ConnectionAcceptor.loop: sockopt(port) error" << std::endl;
      exit(1);
    }

    // bind and listen
    if ((bind(fd_sock, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
      std::cout << "ConnectionAcceptor.loop: bind error" << std::endl;
      exit(1);
    }
    if (listen(fd_sock, MAX_PENDING) < 0) {
      std::cout << "ConnectionAcceptor.loop: listen error" << std::endl;
      exit(1);
    }

    // get connection, spawn thread to handle connection
    while(true) {
      size_t fd_accept{0};
      if ((fd_accept = accept(fd_sock, (struct sockaddr *)&sin, &addr_len)) < 0) {
        std::cout << "ConnectionAcceptor.loop: accept error" << std::endl;
        exit(1);
      }

      std::thread thd([=](){ handshake(fd_accept); });
      thd.detach();
    }
  }
};

int main() {
  ConnectionAcceptor ca("127.0.0.1", 5432, 1);
  ca.run();
}